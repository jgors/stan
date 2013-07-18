data {
  int<lower=0> N; 
  vector[N] dist;
  vector[N] arsenic;
  vector[N] educ;
  int<lower=0,upper=1> switc[N];
}
transformed data {
  vector[N] dist100;
  vector[N] c_dist100;
  vector[N] c_arsenic;
  vector[N] educ4;
  vector[N] inter;
  real mu_dist100;
  real mu_arsenic;
  dist100 <- dist / 100.0;
  mu_dist100 <- mean(dist100);
  mu_arsenic <- mean(arsenic);
  c_dist100 <- dist100 - mu_dist100;
  c_arsenic <- arsenic - mu_arsenic;
  inter <- c_dist100 .* c_arsenic;
  educ4 <- educ / 4.0;
}
parameters {
  vector[5] beta;
} 
model {
  switc ~ bernoulli_logit(beta[1] + beta[2] * c_dist100 
                            + beta[3] * c_arsenic + beta[4] * inter
                            + beta[5] * educ4);
}