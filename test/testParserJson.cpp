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

#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "finalmq/serializejson/ParserJson.h"
#include "finalmq/metadata/MetaData.h"
#include "MockIParserVisitor.h"

//#include <thread>
//#include <chrono>

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::ElementsAreArray;

using namespace finalmq;


MATCHER_P(MatcherMetaField, metaField, "")
{
    return (arg.typeId == metaField.typeId &&
            arg.typeName == metaField.typeName &&
            arg.name == metaField.name);
}


MATCHER_P2(ArrayEq, compareArray, n, "")
{
    return (memcmp(arg, compareArray, n * sizeof(*arg)) == 0);
}



class TestParserJson : public testing::Test
{
public:

protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};





TEST_F(TestParserJson, testUnknownStruct)
{
    MockIParserVisitor mockVisitor;
    std::string data = "{}";
    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, notifyError(data.c_str(), _)).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.BlaBla");
    EXPECT_EQ(res, false);
}




TEST_F(TestParserJson, testBool)
{
    static const bool VALUE = true;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestBool", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestBool", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterBool(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":true}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestBool");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testInt32)
{
    static const int VALUE = -2;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestInt32", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestInt32", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterInt32(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":-2}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestInt32");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testUInt32)
{
    static const int VALUE = 130;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestUInt32", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestUInt32", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterUInt32(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":130}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestUInt32");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testInt64)
{
    static const std::int64_t VALUE = -2;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestInt64", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestInt64", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterInt64(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":-2}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestInt64");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testUInt64)
{
    static const std::uint64_t VALUE = 130;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestUInt64", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestUInt64", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterUInt64(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":130}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestUInt64");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testFloat)
{
    static const float VALUE = -1.1;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestFloat", "value");
    ASSERT_NE(fieldValue, nullptr);

    std::string data = "{\"value\":-1.1}";
    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestFloat", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterFloat(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestFloat");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testDouble)
{
    static const double VALUE = -1.1;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestDouble", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestDouble", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterDouble(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":-1.1}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestDouble");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testString)
{
    static const std::string VALUE = "Hello World";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestString", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestString", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldValue), ArrayEq(VALUE.data(), VALUE.size()), VALUE.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":\"Hello World\"}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestString");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testStdString)
{
    std::string VALUE = "Hello World";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestString", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestString", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldValue), std::move(VALUE))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":\"Hell\\u006f World\"}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestString");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testBytes)
{
    Bytes VALUE = {'H', 'e', 12, 0, 'A'};

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestBytes", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestBytes", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterBytes(MatcherMetaField(*fieldValue), ArrayEq(VALUE.data(), VALUE.size()), VALUE.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":\"He\\u000c\\u0000A\"}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestBytes");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testStruct)
{
    static const std::int32_t VALUE_INT32 = -2;
    static const std::string VALUE_STRING = "Hello World";

    const MetaField* fieldStructInt32 = MetaDataGlobal::instance().getField("test.TestStruct", "struct_int32");
    const MetaField* fieldStructString = MetaDataGlobal::instance().getField("test.TestStruct", "struct_string");
    const MetaField* fieldInt32 = MetaDataGlobal::instance().getField("test.TestInt32", "value");
    const MetaField* fieldString = MetaDataGlobal::instance().getField("test.TestString", "value");
    ASSERT_NE(fieldStructInt32, nullptr);
    ASSERT_NE(fieldStructString, nullptr);
    ASSERT_NE(fieldInt32, nullptr);
    ASSERT_NE(fieldString, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestStruct", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterInt32(MatcherMetaField(*fieldInt32), VALUE_INT32)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldString), ArrayEq(VALUE_STRING.data(), VALUE_STRING.size()), VALUE_STRING.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"struct_int32\":{\"value\":-2},\"struct_string\":{\"value\":\"Hello World\"}}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestStruct");
    EXPECT_EQ(res, true);
}




TEST_F(TestParserJson, testUndefinedStructs)
{
    static const std::int32_t VALUE_INT32 = -2;
    static const std::string VALUE_STRING = "Hello World";

    const MetaField* fieldStructInt32 = MetaDataGlobal::instance().getField("test.TestStruct", "struct_int32");
    const MetaField* fieldStructString = MetaDataGlobal::instance().getField("test.TestStruct", "struct_string");
    const MetaField* fieldInt32 = MetaDataGlobal::instance().getField("test.TestInt32", "value");
    const MetaField* fieldString = MetaDataGlobal::instance().getField("test.TestString", "value");
    ASSERT_NE(fieldStructInt32, nullptr);
    ASSERT_NE(fieldStructString, nullptr);
    ASSERT_NE(fieldInt32, nullptr);
    ASSERT_NE(fieldString, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestStruct", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterInt32(MatcherMetaField(*fieldInt32), VALUE_INT32)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldString), ArrayEq(VALUE_STRING.data(), VALUE_STRING.size()), VALUE_STRING.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"undefined\":{\"undefined\":{\"undefined\":{\"undefined\":{}},\"undefined2\":{\"undefined\":{\"undefined\":1234}}}},\"struct_int32\":{\"value\":-2},\"undefined3\":{\"undefined\":{}},\"struct_string\":{\"value\":\"Hello World\",\"undefine4\":1234,\"undefined5\":{\"undefined\":{}}},\"undefined6\":{\"undefined\":{}}}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestStruct");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testUndefinedValues)
{
    static const std::string VALUE = "Hello World";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestString", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestString", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldValue), ArrayEq(VALUE.data(), VALUE.size()), VALUE.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"unknown1\":1234,\"value\":\"Hello World\",\"unknown2\":1234}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestString");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testEnumAsInt)
{
    static const std::int32_t VALUE = -2;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestEnum", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterEnum(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":-2}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testEnumAsString)
{
    static const std::string VALUE = "FOO_HELLO";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestEnum", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterEnum(MatcherMetaField(*fieldValue), ArrayEq(VALUE.data(), VALUE.size()), VALUE.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":\"FOO_HELLO\"}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}

TEST_F(TestParserJson, testEnumAsStdString)
{
    std::string VALUE = "FOO_HELLO";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestEnum", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterEnum(MatcherMetaField(*fieldValue), std::move(VALUE))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":\"FOO_HELL\\u004f\"}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testEnumNotAvailableInt)
{
    static const std::int32_t VALUE = 42;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestEnum", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterEnum(MatcherMetaField(*fieldValue), VALUE)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":42}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}

TEST_F(TestParserJson, testEnumNotAvailableString)
{
    static const std::string VALUE = "blabla";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestEnum", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterEnum(MatcherMetaField(*fieldValue), ArrayEq(VALUE.data(), VALUE.size()), VALUE.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":\"blabla\"}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}

TEST_F(TestParserJson, testArrayNoArray)
{
    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testArrayUnknownValue)
{
    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"blabla\":[]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestEnum");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayBool)
{
    static const bool VALUE1 = true;
    static const bool VALUE2 = false;
    static const bool VALUE3 = false;
    static const bool VALUE4 = true;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayBool", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayBool", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayBoolMove(MatcherMetaField(*fieldValue), std::vector<bool>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[true,false,false,true]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayBool");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayInt32)
{
    static const std::int32_t VALUE1 = -2;
    static const std::int32_t VALUE2 = 0;
    static const std::int32_t VALUE3 = 2;
    static const std::int32_t VALUE4 = 222;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayInt32", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayInt32", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayInt32(MatcherMetaField(*fieldValue), std::vector<std::int32_t>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[-2,0,2,222]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayInt32");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testArrayUInt32)
{
    static const std::uint32_t VALUE1 = 0xFFFFFFFE;
    static const std::uint32_t VALUE2 = 0;
    static const std::uint32_t VALUE3 = 2;
    static const std::uint32_t VALUE4 = 222;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayUInt32", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayUInt32", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayUInt32(MatcherMetaField(*fieldValue), std::vector<std::uint32_t>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[4294967294,0,2,222]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayUInt32");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayInt64)
{
    static const std::int64_t VALUE1 = -2;
    static const std::int64_t VALUE2 = 0;
    static const std::int64_t VALUE3 = 2;
    static const std::int64_t VALUE4 = 222;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayInt64", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayInt64", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayInt64(MatcherMetaField(*fieldValue), std::vector<std::int64_t>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[-2,0,2,222]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayInt64");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testArrayUInt64)
{
    static const std::uint64_t VALUE1 = 0xFFFFFFFFFFFFFFF;
    static const std::uint64_t VALUE2 = 0;
    static const std::uint64_t VALUE3 = 2;
    static const std::uint64_t VALUE4 = 222;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayUInt64", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayUInt64", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayUInt64(MatcherMetaField(*fieldValue), std::vector<std::uint64_t>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[1152921504606846975,0,2,222]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayUInt64");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayFloat)
{
    static const float VALUE1 = -2.1;
    static const float VALUE2 = 0.0;
    static const float VALUE3 = 2.1;
    static const float VALUE4 = 222.1;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayFloat", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayFloat", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayFloat(MatcherMetaField(*fieldValue), std::vector<float>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[-2.1,0,2.1,222.1]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayFloat");
    EXPECT_EQ(res, true);
}


TEST_F(TestParserJson, testArrayDouble)
{
    static const double VALUE1 = -2.1;
    static const double VALUE2 = 0.0;
    static const double VALUE3 = 2.1;
    static const double VALUE4 = 222.1;

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayDouble", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayDouble", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayDouble(MatcherMetaField(*fieldValue), std::vector<double>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[-2.1,0,2.1,222.1]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayDouble");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayString)
{
    static const std::string VALUE1 = "Hello";
    static const std::string VALUE2 = "";
    static const std::string VALUE3 = "World";
    static const std::string VALUE4 = "Foo";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayString", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayString", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayStringMove(MatcherMetaField(*fieldValue), std::vector<std::string>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[\"Hello\",\"\",\"World\",\"Foo\"]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayString");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayBytes)
{
    static const Bytes VALUE1 = {'H', 'e', '\0', 'l', 'o'};
    static const Bytes VALUE2 = {};
    static const Bytes VALUE3 = {'W', 'o', '\n', '\0', 'd'};
    static const Bytes VALUE4 = {'F', '\t', '\0', 123, 12};

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayBytes", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayBytes", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayBytesMove(MatcherMetaField(*fieldValue), std::vector<Bytes>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[\"He\\u0000lo\",\"\",\"Wo\n\\u0000d\",\"F\t\\u0000\\u007b\\u000c\"]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayBytes");
    EXPECT_EQ(res, true);
}



TEST_F(TestParserJson, testArrayStruct)
{
    static const std::int32_t VALUE1_INT32 = -2;
    static const std::string VALUE1_STRING = "Hello World";
    static const std::int32_t VALUE2_INT32 = 345;
    static const std::string VALUE2_STRING = "foo";

    const MetaField* fieldStruct = MetaDataGlobal::instance().getField("test.TestArrayStruct", "value");
    const MetaField* fieldStructWithoutArray = MetaDataGlobal::instance().getArrayField("test.TestArrayStruct", "value");
    const MetaField* fieldStructInt32 = MetaDataGlobal::instance().getField("test.TestStruct", "struct_int32");
    const MetaField* fieldStructString = MetaDataGlobal::instance().getField("test.TestStruct", "struct_string");
    const MetaField* fieldInt32 = MetaDataGlobal::instance().getField("test.TestInt32", "value");
    const MetaField* fieldString = MetaDataGlobal::instance().getField("test.TestString", "value");
    ASSERT_NE(fieldStruct, nullptr);
    ASSERT_NE(fieldStructWithoutArray, nullptr);
    ASSERT_NE(fieldStructInt32, nullptr);
    ASSERT_NE(fieldStructString, nullptr);
    ASSERT_NE(fieldInt32, nullptr);
    ASSERT_NE(fieldString, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayStruct", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayStruct(MatcherMetaField(*fieldStruct))).Times(1);

        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructWithoutArray))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterInt32(MatcherMetaField(*fieldInt32), VALUE1_INT32)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldString), ArrayEq(VALUE1_STRING.data(), VALUE1_STRING.size()), VALUE1_STRING.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructWithoutArray))).Times(1);

        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructWithoutArray))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterInt32(MatcherMetaField(*fieldInt32), VALUE2_INT32)).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructInt32))).Times(1);
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, enterString(MatcherMetaField(*fieldString), ArrayEq(VALUE2_STRING.data(), VALUE2_STRING.size()), VALUE2_STRING.size())).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructString))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructWithoutArray))).Times(1);

        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(*fieldStructWithoutArray))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(*fieldStructWithoutArray))).Times(1);

        EXPECT_CALL(mockVisitor, exitArrayStruct(MatcherMetaField(*fieldStruct))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[{\"struct_int32\":{\"value\":-2},\"struct_string\":{\"value\":\"Hello World\"}},{\"struct_int32\":{\"value\":345},\"struct_string\":{\"value\":\"foo\"}},{}]}";

    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayStruct");
    EXPECT_EQ(res, true);
}




TEST_F(TestParserJson, testArrayEnum)
{
    static const std::string VALUE1 = "FOO_HELLO";
    static const std::string VALUE2 = "FOO_WORLD";
    static const std::string VALUE3 = "FOO_WORLD2";
    static const std::string VALUE4 = "blabla";

    const MetaField* fieldValue = MetaDataGlobal::instance().getField("test.TestArrayEnum", "value");
    ASSERT_NE(fieldValue, nullptr);

    MockIParserVisitor mockVisitor;
    MetaField rootStruct = {MetaTypeId::TYPE_STRUCT, "test.TestArrayEnum", ""};

    {
        testing::InSequence seq;
        EXPECT_CALL(mockVisitor, enterStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, enterArrayEnum(MatcherMetaField(*fieldValue), std::vector<std::string>({VALUE1, VALUE2, VALUE3, VALUE4}))).Times(1);
        EXPECT_CALL(mockVisitor, exitStruct(MatcherMetaField(rootStruct))).Times(1);
        EXPECT_CALL(mockVisitor, finished()).Times(1);
    }

    std::string data = "{\"value\":[\"FOO_HELLO\",\"FOO_WORLD\",\"FOO_WORLD2\",\"blabla\"]}";
    ParserJson parser(mockVisitor, data.data(), data.size());
    bool res = parser.parseStruct("test.TestArrayEnum");
    EXPECT_EQ(res, true);
}



