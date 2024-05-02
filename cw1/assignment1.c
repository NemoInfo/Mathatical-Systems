#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
// ADDED
#include <time.h>

union DoubleToInt {
  double dVal;
  uint64_t iVal;
};

/*
  A function that rounds a binary64 value to a binary32 value
  stochastically. Implemented by treating FP number representations
  as integer values.
*/
float SR(double x) {
  union DoubleToInt temp;
  temp.dVal = x;
  uint32_t r = rand() & 0x1FFFFFFF;
  temp.iVal += r;
  temp.iVal = temp.iVal & 0xFFFFFFFFE0000000;

  return (float)temp.dVal;
}

void fastTwoSum (float a, float b, float *s, float *t) {
  float temp;

  *s = a + b;
  temp = *s - a;
  *t = b - temp;
}

/* --------------------------------- */
/*              PART 1               */
/* --------------------------------- */


void RA_RZ(double x, float *ra, float *rz) {
  float f1 = (float) x;
  float f2 = nexttowardf(f1, x);

  if (f1 > f2 == x > 0.0 ){
    *ra = f1;
    *rz = f2;
  } else {
    *ra = f2;
    *rz = f1;
  }
}

// Implement SR_alternative according to the Eqn 1.
float SR_alternative(double x) {
  float ra, rz;
  RA_RZ(x, &ra, &rz);

  if(ra == rz){
    return ra;
  }

  double p = (x - rz) / (ra - rz);
  double P = (double) rand() / RAND_MAX;

  return (P < p) ? ra : rz;
}


// Uncomment any of the bellow to save outptu to file
// #define SAVE_PART1 "part1_out.txt"
// #define SAVE_PART2 "part2_out.txt"
// #define SAVE_PART3 "part3_out.txt"


// Part 1
#define K 5000000
// Part 2
#define N 500000000
// Part 3
#define E 500000000

#ifdef SAVE_PART1
// Step between mae samples
#define I1 1000
// Number of mae samples
#define NUM_MAE K / I1 + 1
double mae_def[NUM_MAE];
double mae_alt[NUM_MAE];
#endif


#ifdef SAVE_PART2
// Step between error samples
#define I2 1000000
// Number of error samples
#define NUM_AE N / I2 + 1
double ae_rrn[NUM_AE]; // Recursive Sum, RN
double ae_rsr[NUM_AE]; // Recusrive Sum, SN
double ae_csm[NUM_AE]; // Compensated Sum
#endif


#ifdef SAVE_PART3
// Step between error samples
#define I3 100000
// Number of error samples
#define NUM_ZAE E / I3 + 1
double zae_rrn[NUM_ZAE]; // Recursive Sum, RN
double zae_rsr[NUM_ZAE]; // Recusrive Sum, SN
double zae_csm[NUM_ZAE]; // Compensated Sum
#endif

int main() {
  srand(0);

  // An arbitrary value for rounding.
  double sample = M_PI;
  double avg_def = 0;
  double avg_alt = 0;

  // Calculate the neighbouring binary32 values.
  float closest = (float)sample;
  float hi, lo;
  if (closest > sample) {
    hi = nextafterf(closest, -INFINITY);
    lo = closest;
  }
  else {
    lo = nextafterf(closest, INFINITY);
    hi = closest;
  }

  // Round many times, and calculate the average values as well as count
  // the numbers of times rounding was up/down.
  int num_hi_def = 0;
  int num_lo_def = 0;
  int num_hi_alt = 0;
  int num_lo_alt = 0;

  for (int i = 1; i <= K; i++) {
    float r_def = SR(sample);
    num_hi_def += r_def == lo;
    num_lo_def += r_def == hi;
    avg_def += (double) r_def / K;

    float r_alt = SR_alternative(sample);
    num_hi_alt += r_alt == lo;
    num_lo_alt += r_alt == hi;
    avg_alt += (double) r_alt / K;


    // Record MAE statistics
    #ifdef SAVE_PART1
    if(i % I1 == 0) {
      int j = i / I1;
      mae_alt[j] = fabs(sample - avg_def * K / i );
      mae_def[j] = fabs(sample - avg_alt * K / i);
    }
    #endif
  }


  // Print out some useful stats.
  printf("Value being rounded:           %.60f \n", sample);
  printf("SR average value:              %.60f \n", avg_def);
  printf("SR_alternative average value:  %.60f \n", avg_alt);
  printf("Binary32 value before:         %.60f \n", hi);
  printf("Binary32 value after:          %.60f \n", lo);
  printf("Closest binary32:              %.60f \n", closest);


  #ifdef SAVE_PART1
  // Gather average value stats
  FILE *part1_out = fopen(SAVE_PART1, "w");
  if (part1_out == NULL) {
    printf("Failed to open output file");
    return -1;
  }

  for (int i = 1; i < NUM_MAE; i++) {
    fprintf(part1_out, "%.60f %.60f\n", mae_def[i], mae_alt[i]);
  }

  printf("Saved part 1 data to file %s\n", SAVE_PART1);
  fclose(part1_out);
  #endif

  // Check that SR_alternative function is correct by comparing the probabilities
  // of rounding up/down, and the expected probability. Print them out
  // below.

  double p_up = (double) num_hi_def / (double) (num_hi_def + num_lo_def);
  double p_up_alt = (double) num_hi_alt / (double) (num_hi_alt + num_lo_alt);
  double p_up_exp = (sample - hi) / (lo - hi);

  printf("p_exp: %.60f\n", p_up_exp);
  printf("p_def: %.60f\n", p_up);
  printf("p_alt: %.60f\n", p_up_alt);

  /* --------------------------------- */
  /*              PART 2               */
  /* --------------------------------- */

  float fharmonic = 0;
  float fharmonic_sr = 0;
  float fharmonic_comp = 0;
  double dharmonic = 0;

  // Error term in the compensated summation.
  float t = 0;

  int stagnated = 0;
  for (int i = 1; i <= N; i++) {
    // Recursive sum, binary32 RN
    if(!stagnated && fharmonic == fharmonic + (float)1/i) {
      stagnated = i;
    }
    fharmonic += (float)1/i;
    // Recursive sum, binary32 SR
    fharmonic_sr = SR_alternative((double)fharmonic_sr + (double)1/i);
    // Compensated sum, binary32
    fastTwoSum(fharmonic_comp, (float)1/i + t, &fharmonic_comp, &t);
    // Recursive sum, binary64 RN
    dharmonic += (double)1/i;

    #ifdef SAVE_PART2
    if (i % I2 == 0) {
      int j = i / I2;
      ae_rrn[j] = fabs(dharmonic - fharmonic);
      ae_rsr[j] = fabs(dharmonic - fharmonic_sr);
      ae_csm[j] = fabs(dharmonic - fharmonic_comp);
    }
    #endif
  }

  printf("Values of the harmonic series after %d iterations \n", N);
  printf("Recursive summation, binary32:          %.30f \n", fharmonic);
  printf("Recursive summation with SR, binary32:  %.30f \n", fharmonic_sr);
  printf("Compensated summation, binary32:        %.30f \n", fharmonic_comp);
  printf("Recursive summation, binary64:          %.30f \n", dharmonic);
  printf("RS binary32 stagnated at:               %d \n", stagnated);


  #ifdef SAVE_PART2
  // Gather average value stats
  FILE *part2_out = fopen(SAVE_PART2, "w");
  if (part2_out == NULL) {
    printf("Failed to open output file");
    return -1;
  }

  for (int i = 1; i < NUM_AE; i++) {
    fprintf(part2_out, "%.60f %.60f %.60f\n", ae_rrn[i], ae_rsr[i], ae_csm[i]);
  }

  printf("Saved part 2 data to file %s\n", SAVE_PART2);
  fclose(part2_out);
  #endif


  /* --------------------------------- */
  /*              PART 3               */
  /* --------------------------------- */

  float fzeta_rn = 0;
  float fzeta_sr = 0;
  float fzeta_cs = 0;
  double dzeta_rn = 0;

  // Error term in the compensated summation.
  float zt = 0;

  for (long int i = 1; i <= E; i++) {
    // Recursive sum, binary32 RN
    fzeta_rn += (float)1/(float)(i*i);
    // Recursive sum, binary32 SR
    fzeta_sr = SR_alternative((double)fzeta_sr + (double)1/(i*i));
    // Compensated sum, binary32
    fastTwoSum(fzeta_cs, (float)1/(i*i) + zt, &fzeta_cs, &zt);
    // Recursive sum, binary64 RN
    dzeta_rn += (double)1/(i*i);

    #ifdef SAVE_PART3
    if (i % I3 == 0) {
      int j = i / I3;
      zae_rrn[j] = fabs(dzeta_rn - fzeta_rn);
      zae_rsr[j] = fabs(dzeta_rn - fzeta_sr);
      zae_csm[j] = fabs(dzeta_rn - fzeta_cs);
    }
    #endif
  }

  printf("Values of the Reimann zeta series after %d iterations \n", E);
  printf("Recursive summation, binary32:          %.60f \n", fzeta_rn);
  printf("Recursive summation with SR, binary32:  %.60f \n", fzeta_sr);
  printf("Compensated summation, binary32:        %.60f \n", fzeta_cs);
  printf("Recursive summation, binary64:          %.60f \n", dzeta_rn);



  #ifdef SAVE_PART3
  // Gather average value stats
  FILE *part3_out = fopen(SAVE_PART3, "w");
  if (part3_out == NULL) {
    printf("Failed to open output file");
    return -1;
  }


  for (int i = 1; i < NUM_ZAE; i++) {
    fprintf(part3_out, "%.60f %.60f %.60f\n", zae_rrn[i], zae_rsr[i], zae_csm[i]);
  }


  printf("Saved part 3 data to file %s\n", SAVE_PART3);
  fclose(part3_out);
  #endif

  return 0;
}
