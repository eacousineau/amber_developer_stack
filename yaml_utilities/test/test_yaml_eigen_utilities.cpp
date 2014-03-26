#include <gtest/gtest.h>

#include <common_assert.hpp>
#include <stl_utilities/container_utilities.hpp>
#include <yaml_eigen_utilities/yaml_utilities.hpp>
#include <yaml_eigen_utilities/yaml_eigen_utilities.hpp>

using namespace std;
using namespace YAML;
using namespace yaml_eigen_utilities;

using Eigen::VectorXd;
using Eigen::MatrixXd;

/**
 * @brief BasicRead Read a list of strings encoded as a YAML string
 */
TEST(yaml_utilities, BasicRead)
{
    Node node;
    yaml_read_string("[a, b, c]", node);
    vector<string> actual, expected;
    node >> actual;
    expected << "a" << "b" << "c";
    EXPECT_EQ(expected, actual);
}

/**
 * @brief BasicWrite Do the reverse
 */
TEST(yaml_utilities, BasicWrite)
{
    string actual, expected = "[a, b, c]";
    vector<string> test;
    test << "a" << "b" << "c";
    Emitter out;
    out << Flow << test;
    yaml_write_string(actual, out);
    EXPECT_EQ(expected, actual);
}

/**
 * @brief VectorRead Read a vector of doubles into a dynamically sized vector
 */
TEST(yaml_utilities, VectorRead)
{
    Node node;
    yaml_read_string("[1, 2, 3]", node);
    VectorXd actual, expected(3);
    expected << 1, 2, 3;
    node >> actual;
    EXPECT_EQ(expected, actual);
}

/**
 * @brief MatrixWrite Write a dynamically sized matrix
 */
TEST(yaml_utilities, MatrixWrite)
{
    string actual, expected =
            "- [1, 2]\n"
            "- [3, 4]";
    MatrixXd X(2, 2);
    X <<
            1, 2,
               3, 4;
    Emitter out;
    out << X;
    yaml_write_string(actual, out);
    EXPECT_EQ(expected, actual);
}

/**
 * @brief MatrixRead Read a dynamically sized matrix
 */
TEST(yaml_utilities, MatrixRead)
{
    Node node;
    yaml_read_string("[[1, 2], [3, 4]]", node);
    MatrixXd actual, expected(2, 2);
    expected <<
            1, 2,
               3, 4;
    node >> actual;
    EXPECT_EQ(expected, actual);
}

/**
 * @brief FileWriteAndReadStaticSized Test reading and writing files as well as statically-sized matrices
 */
TEST(yaml_utilities, FileWriteAndReadStaticSized)
{
    string name_expected = "Test", name_actual;
    Eigen::Matrix4d A_expected, A_actual;
    A_expected <<
        1, 2, 3, 0,
        4, 5, 6, 0,
        7, 8, 9, 0,
        0, 0, 0, 1;
    Eigen::Matrix<double, 4, 1> x_expected, x_actual;
    x_expected << 1, 2, 3, 0;
    
    // Write
    Emitter out;
    out << BeginMap
        << Key << "name" << Value << name_expected
        << Key << "A" << Value << A_expected
        << Key << "x" << Value << x_expected
        << EndMap;
    
    boost::filesystem::path filePath = "/tmp/yaml_utilities-test.yml";
    yaml_write_file(filePath, out);
    
    // Read
    Node node;
    yaml_read_file(filePath, node);
    node["name"] >> name_actual;
    EXPECT_EQ(name_expected, name_actual);
    node["A"] >> A_actual;
    EXPECT_EQ(A_expected, A_actual);
    node["x"] >> x_actual;
    EXPECT_EQ(x_expected, x_actual);
}

/**
 * @brief FileDoesNotExistTest Ensure that an exception is thrown if we try to read a nonexistant file
 */
TEST(yaml_utilities, FileDoesNotExistTest)
{
    Node node;
    boost::filesystem::path badFilePath = "/tmp/I_HOPE_THIS_FILE_DOES_NOT_EXIST_ON_YOUR_SYSTEM_OTHERWISE_THIS_TEST_WILL_BREAK_SHOULD_WE_USE_AN_INVALID_FILE_NAME_INSTEAD";
    EXPECT_THROW(yaml_read_file(badFilePath, node), runtime_error);
}

/**
 * @brief BadDereferenceTest Ensure that we get a YAML::BadDereference exception if reading a null node
 */
TEST(yaml_utilities, BadDereferenceTest)
{
    Node node;
    yaml_read_string("", node);
    EXPECT_THROW(node["something"].to<int>(), BadDereference);
    EXPECT_EQ(NodeType::Null, node.Type());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
