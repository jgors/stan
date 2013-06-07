#include <gtest/gtest.h>
#include <stan/agrad/fvar.hpp>
#include <stan/math/functions/inv_square.hpp>
#include <stan/math/constants.hpp>
#include <stan/agrad/var.hpp>
#include <test/agrad/util.hpp>

TEST(AgradFvar, inv_square) {
  using stan::agrad::fvar;
  using stan::math::inv_square;

  fvar<double> x(0.5);
  x.d_ = 1.0;   // derivatives w.r.t. x
  fvar<double> a = inv_square(x);

  EXPECT_FLOAT_EQ(inv_square(0.5), a.val_);
  EXPECT_FLOAT_EQ(-2 / (0.5 * 0.5 * 0.5), a.d_);

  fvar<double> z(0.0);
  z.d_ = 1.0;
  fvar<double> g = inv_square(z);
  EXPECT_FLOAT_EQ(stan::math::positive_infinity(), g.val_);
  EXPECT_FLOAT_EQ(stan::math::negative_infinity(), g.d_);
}   

TEST(AgradFvarVar, inv_square) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using stan::math::inv_square;

  fvar<var> x(0.5,1.0);
  fvar<var> a = inv_square(x);

  EXPECT_FLOAT_EQ(inv_square(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(-2.0 * inv_square(0.5) / (0.5), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(-2.0 / (0.5 * 0.5 * 0.5), g[0]);
}

TEST(AgradFvarFvar, inv_square) {
  using stan::agrad::fvar;
  using stan::math::inv_square;
  using std::log;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = inv_square(x);

  EXPECT_FLOAT_EQ(inv_square(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(-2.0 * inv_square(0.5) / (0.5), a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}