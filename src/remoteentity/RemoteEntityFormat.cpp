//MIT License

//Copyright (c) 2020 bexoft GmbH (mail@bexoft.de)

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include "finalmq/remoteentity/RemoteEntityFormat.h"
#include "finalmq/remoteentity/entitydata.fmq.h"

#include "finalmq/serializeproto/SerializerProto.h"
#include "finalmq/serializejson/SerializerJson.h"
#include "finalmq/serializestruct/ParserStruct.h"

#include "finalmq/serializeproto/ParserProto.h"
#include "finalmq/serializejson/ParserJson.h"
#include "finalmq/serializestruct/SerializerStruct.h"
#include "finalmq/serializestruct/StructFactoryRegistry.h"

#include "finalmq/helpers/ModulenameFinalmq.h"


using finalmq::remoteentity::MsgMode;
using finalmq::remoteentity::Status;
using finalmq::remoteentity::Header;


namespace finalmq {


void RemoteEntityFormat::serializeProto(IMessage& message, const Header& header)
{
    char* bufferSizeHeader = message.addSendPayload(2048);
    message.downsizeLastSendPayload(4);

    SerializerProto serializerHeader(message);
    ParserStruct parserHeader(serializerHeader, header);
    parserHeader.parseStruct();
    ssize_t sizeHeader = message.getTotalSendPayloadSize() - 4;
    assert(sizeHeader >= 0);

    *bufferSizeHeader = static_cast<unsigned char>(sizeHeader);
    ++bufferSizeHeader;
    *bufferSizeHeader = static_cast<unsigned char>(sizeHeader >> 8);
    ++bufferSizeHeader;
    *bufferSizeHeader = static_cast<unsigned char>(sizeHeader >> 16);
    ++bufferSizeHeader;
    *bufferSizeHeader = static_cast<unsigned char>(sizeHeader >> 24);
}



void RemoteEntityFormat::serializeProto(IMessage& message, const Header& header, const StructBase& structBase)
{
    serializeProto(message, header);

    const std::string* typeName = &structBase.getStructInfo().getTypeName();
    if (*typeName == remoteentity::GenericMessage::structInfo().getTypeName())
    {
        const remoteentity::GenericMessage& genericMessage = static_cast<const remoteentity::GenericMessage&>(structBase);
        assert(genericMessage.contenttype == RemoteEntityContentType::CONTENTTYPE_PROTO);
        char* payload = message.addSendPayload(genericMessage.data.size());
        memcpy(payload, genericMessage.data.data(), genericMessage.data.size());
    }
    else
    {
        SerializerProto serializerData(message);
        ParserStruct parserData(serializerData, structBase);
        parserData.parseStruct();
    }
}

#define JSONBLOCKSIZE   2048

void RemoteEntityFormat::serializeJson(IMessage& message, const Header& header, bool structBaseAvailable)
{
    message.addSendPayload("[", 1, JSONBLOCKSIZE);

    SerializerJson serializerHeader(message);
    ParserStruct parserHeader(serializerHeader, header);
    parserHeader.parseStruct();


    // add end of header
    if (structBaseAvailable)
    {
        message.addSendPayload(",\t", 2, JSONBLOCKSIZE);
    }
    else
    {
        message.addSendPayload(",\t{}]", 5);
    }
}



void RemoteEntityFormat::serializeJson(IMessage& message, const Header& header, const StructBase& structBase)
{
    serializeJson(message, header, true);

    const std::string* typeName = &structBase.getStructInfo().getTypeName();
    if (*typeName == remoteentity::GenericMessage::structInfo().getTypeName())
    {
        const remoteentity::GenericMessage& genericMessage = static_cast<const remoteentity::GenericMessage&>(structBase);
        assert(genericMessage.contenttype == RemoteEntityContentType::CONTENTTYPE_JSON);
        char* payload = message.addSendPayload(genericMessage.data.size() + 1);
        memcpy(payload, genericMessage.data.data(), genericMessage.data.size());
        payload[genericMessage.data.size()] = ']';
    }
    else
    {
        SerializerJson serializerData(message);
        ParserStruct parserData(serializerData, structBase);
        parserData.parseStruct();
        message.addSendPayload("]", 1);
    }
}



void RemoteEntityFormat::serialize(IMessage& message, int contentType, const remoteentity::Header& header)
{
    switch (contentType)
    {
    case RemoteEntityContentType::CONTENTTYPE_PROTO:
        serializeProto(message, header);
        break;
    case RemoteEntityContentType::CONTENTTYPE_JSON:
        serializeJson(message, header, false);
        break;
    default:
        assert(false);
        break;
    }
}


void RemoteEntityFormat::serialize(IMessage& message, int contentType, const remoteentity::Header& header, const StructBase& structBase)
{
    switch (contentType)
    {
    case RemoteEntityContentType::CONTENTTYPE_PROTO:
        serializeProto(message, header, structBase);
        break;
    case RemoteEntityContentType::CONTENTTYPE_JSON:
        serializeJson(message, header, structBase);
        break;
    default:
        assert(false);
        break;
    }
}


inline static bool shallSend(const remoteentity::Header& header)
{
    if ((header.mode != MsgMode::MSG_REPLY) ||
        (header.status == Status::STATUS_ENTITY_NOT_FOUND || header.corrid != CORRELATIONID_NONE))
    {
        return true;
    }
    return false;
}




bool RemoteEntityFormat::send(const IProtocolSessionPtr& session, const remoteentity::Header& header, const StructBase& structBase)
{
    bool ok = true;
    if (shallSend(header))
    {
        assert(session);
        IMessagePtr message = session->createMessage();
        assert(message);
        serialize(*message, session->getContentType(), header, structBase);
        ok = session->sendMessage(message);
    }
    return ok;
}




bool RemoteEntityFormat::send(const IProtocolSessionPtr& session, const remoteentity::Header& header)
{
    bool ok = true;
    if (shallSend(header))
    {
        assert(session);
        IMessagePtr message = session->createMessage();
        assert(message);
        serialize(*message, session->getContentType(), header);
        ok = session->sendMessage(message);
    }
    return ok;
}



std::shared_ptr<StructBase> RemoteEntityFormat::parseMessageProto(const BufferRef& bufferRef, Header& header, bool& syntaxError)
{
    syntaxError = false;
    const char* buffer = bufferRef.first;
    ssize_t sizeBuffer = bufferRef.second;
    if (sizeBuffer < 4)
    {
        streamError << "buffer size too small: " << sizeBuffer;
        return nullptr;
    }

    ssize_t sizePayload = sizeBuffer - 4;
    ssize_t sizeHeader = 0;
    if (sizeBuffer >= 4)
    {
        sizeHeader = (unsigned char)*buffer;
        ++buffer;
        sizeHeader |= ((unsigned char)*buffer) << 8;
        ++buffer;
        sizeHeader |= ((unsigned char)*buffer) << 16;
        ++buffer;
        sizeHeader |= ((unsigned char)*buffer) << 24;
        ++buffer;
    }
    bool ok = false;
    std::shared_ptr<StructBase> data;

    if (sizeHeader <= sizePayload)
    {
        SerializerStruct serializerHeader(header);
        ParserProto parserHeader(serializerHeader, buffer, sizeHeader);
        ok = parserHeader.parseStruct(Header::structInfo().getTypeName());
    }

    if (ok && !header.type.empty())
    {
        data = StructFactoryRegistry::instance().createStruct(header.type);

        ssize_t sizeData = sizePayload - sizeHeader;
        buffer += sizeHeader;

        if (data)
        {
            assert(sizeData >= 0);
            SerializerStruct serializerData(*data);
            ParserProto parserData(serializerData, buffer, sizeData);
            ok = parserData.parseStruct(header.type);
            if (!ok)
            {
                syntaxError = true;
                data = nullptr;
            }
        }
        else
        {
            std::shared_ptr<remoteentity::GenericMessage> genericMessage = std::make_shared<remoteentity::GenericMessage>();
            genericMessage->type = header.type;
            genericMessage->contenttype = RemoteEntityContentType::CONTENTTYPE_PROTO;
            genericMessage->data.resize(sizeData);
            memcpy(genericMessage->data.data(), buffer, sizeData);
            data = genericMessage;
        }
    }

    return data;
}

static ssize_t findFirst(const char* buffer, ssize_t size, char c)
{
    for (ssize_t i = 0; i < size; ++i)
    {
        if (buffer[i] == c)
        {
            return i;
        }
    }
    return -1;
}

static ssize_t findLast(const char* buffer, ssize_t size, char c)
{
    for (ssize_t i = size - 1; i >= 0; --i)
    {
        if (buffer[i] == c)
        {
            return i;
        }
    }
    return -1;
}


std::shared_ptr<StructBase> RemoteEntityFormat::parseMessageJson(const BufferRef& bufferRef, Header& header, bool& syntaxError)
{
    syntaxError = false;
    const char* buffer = bufferRef.first;
    ssize_t sizeBuffer = bufferRef.second;

    if (sizeBuffer == 0)
    {
        return nullptr;
    }

    if (buffer[0] == '[')
    {
        ++buffer;
        --sizeBuffer;
    }
    if (buffer[sizeBuffer - 1] == ']')
    {
        --sizeBuffer;
    }

    const char* endHeader = nullptr;
    if (buffer[0] == '/')
    {
        // 012345678901234567890123456789
        // /MyServer/test.TestRequest#1{}
        ssize_t ixEndHeader = findFirst(buffer, sizeBuffer, '{');   //28
        if (ixEndHeader == -1)
        {
            ixEndHeader = sizeBuffer;
        }
        endHeader = &buffer[ixEndHeader];
        ssize_t ixStartCommand = findLast(buffer, ixEndHeader, '/');    //9
        assert(ixStartCommand >= 0);
        if (ixStartCommand > 1)
        {
            header.destname = {&buffer[1], &buffer[ixStartCommand]};
        }
        ssize_t ixCorrelationId = findLast(buffer, ixEndHeader, '#');   //26
        if (ixCorrelationId != -1)
        {
            header.corrid = strtoll(&buffer[ixCorrelationId+1], nullptr, 10);
        }
        else
        {
            ixCorrelationId = ixEndHeader;
        }
        header.type = {&buffer[ixStartCommand+1], &buffer[ixCorrelationId]};

        header.mode = MsgMode::MSG_REQUEST;
    }
    else
    {
        SerializerStruct serializerHeader(header);
        ParserJson parserHeader(serializerHeader, buffer, sizeBuffer);
        endHeader = parserHeader.parseStruct(header.getStructInfo().getTypeName());
        // skip comma
        ++endHeader;
    }

    std::shared_ptr<StructBase> data;

    if (endHeader && !header.type.empty())
    {
        data = StructFactoryRegistry::instance().createStruct(header.type);

        assert(endHeader);
        ssize_t sizeHeader = endHeader - buffer;
        assert(sizeHeader >= 0);
        buffer += sizeHeader;
        ssize_t sizeData = sizeBuffer - sizeHeader;
        assert(sizeData >= 0);

        if (data)
        {
            if (sizeData > 0)
            {
                SerializerStruct serializerData(*data);
                ParserJson parserData(serializerData, buffer, sizeData);
                const char* endData = parserData.parseStruct(header.type);
                if (!endData)
                {
                    syntaxError = true;
                    data = nullptr;
                }
            }
        }
        else
        {
            std::shared_ptr<remoteentity::GenericMessage> genericMessage = std::make_shared<remoteentity::GenericMessage>();
            genericMessage->type = header.type;
            genericMessage->contenttype = RemoteEntityContentType::CONTENTTYPE_JSON;
            genericMessage->data.resize(sizeData);
            memcpy(genericMessage->data.data(), buffer, sizeData);
            data = genericMessage;
        }
    }

    return data;
}



std::shared_ptr<StructBase> RemoteEntityFormat::parseMessage(const IMessage& message, int contentType, Header& header, bool& syntaxError)
{
    syntaxError = false;
    BufferRef bufferRef = message.getReceivePayload();
    std::shared_ptr<StructBase> structBase;

    switch (contentType)
    {
    case RemoteEntityContentType::CONTENTTYPE_PROTO:
        structBase = parseMessageProto(bufferRef, header, syntaxError);
        break;
    case RemoteEntityContentType::CONTENTTYPE_JSON:
        structBase = parseMessageJson(bufferRef, header, syntaxError);
        break;
    default:
        assert(false);
        break;
    }

    return structBase;
}



}   // namespace finalmq