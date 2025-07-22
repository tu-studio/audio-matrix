#include <gtest/gtest.h>
#include <CustomAtomic.h>
#include <vector>
TEST(CustomAtomic, simple_test){
 
    std::vector<custom_atomic<double>> vec;
    vec.push_back(custom_atomic<double>());
    vec.push_back(custom_atomic<double>(5.0));
    vec.push_back(custom_atomic<double>(7.0));
    ASSERT_DOUBLE_EQ(vec[0].load(), 0.0);
    ASSERT_DOUBLE_EQ(vec[1].load(), 5.0);
    ASSERT_DOUBLE_EQ(vec[2].load(), 7.0);

    vec[0].store(10);
    ASSERT_DOUBLE_EQ(vec[0].load(), 10.0);
    

}