#include <config.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <gpg-error.h>

#include "types.h"
#include "g10lib.h"
#include "gcrypt-int.h"

/*
  Code from:

  Repository: https://github.com/pq-crystals/kyber.git
  Branch: standard
  Commit: 11d00ff1f20cfca1f72d819e5a45165c1e0a2816

  Licence:
  Public Domain (https://creativecommons.org/share-your-work/public-domain/cc0/);
  or Apache 2.0 License (https://www.apache.org/licenses/LICENSE-2.0.html).

  Authors:
	Joppe Bos
	Léo Ducas
	Eike Kiltz
        Tancrède Lepoint
	Vadim Lyubashevsky
	John Schanck
	Peter Schwabe
        Gregor Seiler
	Damien Stehlé

  Kyber Home: https://www.pq-crystals.org/kyber/
 */

/*************** kyber/ref/fips202.h */
#define SHAKE128_RATE 168

typedef struct {
  uint64_t s[25];
  unsigned int pos;
} keccak_state;

void shake128_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen);
void shake128_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state);

void shake256_init(keccak_state *state);
void shake256_absorb(keccak_state *state, const uint8_t *in, size_t inlen);
void shake256_finalize(keccak_state *state);
void shake256_squeeze(uint8_t *out, size_t outlen, keccak_state *state);

void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen);
void sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen);
void sha3_512(uint8_t h[64], const uint8_t *in, size_t inlen);

/*************** kyber/ref/params.h */
#undef KYBER_K

#define KYBER_N 256
#define KYBER_Q 3329

#define KYBER_SYMBYTES 32   /* size in bytes of hashes, and seeds */
#define KYBER_SSBYTES  32   /* size in bytes of shared key */

#define KYBER_POLYBYTES		384
//#define KYBER_POLYVECBYTES	(KYBER_K * KYBER_POLYBYTES)
#define KYBER_POLYVECBYTES2	(2 * KYBER_POLYBYTES)
#define KYBER_POLYVECBYTES3	(3 * KYBER_POLYBYTES)
#define KYBER_POLYVECBYTES4	(4 * KYBER_POLYBYTES)

#define KYBER_ETA1_2   3
#define KYBER_ETA1_3_4 2

#if 0
#if KYBER_K == 2
#define KYBER_POLYCOMPRESSEDBYTES    128
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 320)
#elif KYBER_K == 3
#define KYBER_POLYCOMPRESSEDBYTES    128
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 320)
#elif KYBER_K == 4
#define KYBER_POLYCOMPRESSEDBYTES    160
#define KYBER_POLYVECCOMPRESSEDBYTES (KYBER_K * 352)
#endif
#endif
#define KYBER_POLYCOMPRESSEDBYTES2   128
#define KYBER_POLYCOMPRESSEDBYTES3   128
#define KYBER_POLYCOMPRESSEDBYTES4   160
#define KYBER_POLYVECCOMPRESSEDBYTES2 (2 * 320)
#define KYBER_POLYVECCOMPRESSEDBYTES3 (3 * 320)
#define KYBER_POLYVECCOMPRESSEDBYTES4 (4 * 352)

#define KYBER_ETA2 2

#define KYBER_INDCPA_MSGBYTES       (KYBER_SYMBYTES)
//#define KYBER_INDCPA_PUBLICKEYBYTES (KYBER_POLYVECBYTES + KYBER_SYMBYTES)
#define KYBER_INDCPA_PUBLICKEYBYTES2 (KYBER_POLYVECBYTES2 + KYBER_SYMBYTES)
#define KYBER_INDCPA_PUBLICKEYBYTES3 (KYBER_POLYVECBYTES3 + KYBER_SYMBYTES)
#define KYBER_INDCPA_PUBLICKEYBYTES4 (KYBER_POLYVECBYTES4 + KYBER_SYMBYTES)

//#define KYBER_INDCPA_SECRETKEYBYTES (KYBER_POLYVECBYTES)
#define KYBER_INDCPA_SECRETKEYBYTES2 (KYBER_POLYVECBYTES2)
#define KYBER_INDCPA_SECRETKEYBYTES3 (KYBER_POLYVECBYTES3)
#define KYBER_INDCPA_SECRETKEYBYTES4 (KYBER_POLYVECBYTES4)
//#define KYBER_INDCPA_BYTES          (KYBER_POLYVECCOMPRESSEDBYTES + KYBER_POLYCOMPRESSEDBYTES)
#define KYBER_INDCPA_BYTES2      (KYBER_POLYVECCOMPRESSEDBYTES2 + KYBER_POLYCOMPRESSEDBYTES2)
#define KYBER_INDCPA_BYTES3      (KYBER_POLYVECCOMPRESSEDBYTES3 + KYBER_POLYCOMPRESSEDBYTES3)
#define KYBER_INDCPA_BYTES4      (KYBER_POLYVECCOMPRESSEDBYTES4 + KYBER_POLYCOMPRESSEDBYTES4)

//#define KYBER_PUBLICKEYBYTES  (KYBER_INDCPA_PUBLICKEYBYTES)
#define KYBER_PUBLICKEYBYTES2  (KYBER_INDCPA_PUBLICKEYBYTES2)
#define KYBER_PUBLICKEYBYTES3  (KYBER_INDCPA_PUBLICKEYBYTES3)
#define KYBER_PUBLICKEYBYTES4  (KYBER_INDCPA_PUBLICKEYBYTES4)
/* 32 bytes of additional space to save H(pk) */
//#define KYBER_SECRETKEYBYTES  (KYBER_INDCPA_SECRETKEYBYTES + KYBER_INDCPA_PUBLICKEYBYTES + 2*KYBER_SYMBYTES)
#define KYBER_SECRETKEYBYTES2  (KYBER_INDCPA_SECRETKEYBYTES2 + KYBER_INDCPA_PUBLICKEYBYTES2 + 2*KYBER_SYMBYTES)
#define KYBER_SECRETKEYBYTES3  (KYBER_INDCPA_SECRETKEYBYTES3 + KYBER_INDCPA_PUBLICKEYBYTES3 + 2*KYBER_SYMBYTES)
#define KYBER_SECRETKEYBYTES4  (KYBER_INDCPA_SECRETKEYBYTES4 + KYBER_INDCPA_PUBLICKEYBYTES4 + 2*KYBER_SYMBYTES)
//#define KYBER_CIPHERTEXTBYTES (KYBER_INDCPA_BYTES)
#define KYBER_CIPHERTEXTBYTES2 (KYBER_INDCPA_BYTES2)
#define KYBER_CIPHERTEXTBYTES3 (KYBER_INDCPA_BYTES3)
#define KYBER_CIPHERTEXTBYTES4 (KYBER_INDCPA_BYTES4)

/*************** kyber/ref/poly.h */
/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*coeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct{
  int16_t coeffs[KYBER_N];
} poly;

static void poly_compress_128(uint8_t r[128], const poly *a);
static void poly_compress_160(uint8_t r[160], const poly *a);
static void poly_decompress_128(poly *r, const uint8_t a[128]);
static void poly_decompress_160(poly *r, const uint8_t a[160]);

static void poly_tobytes(uint8_t r[KYBER_POLYBYTES], const poly *a);
static void poly_frombytes(poly *r, const uint8_t a[KYBER_POLYBYTES]);

static void poly_frommsg(poly *r, const uint8_t msg[KYBER_INDCPA_MSGBYTES]);
static void poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], const poly *r);

static void poly_getnoise_eta1_2(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce);
static void poly_getnoise_eta1_3_4(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce);

static void poly_getnoise_eta2(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce);

static void poly_ntt(poly *r);
static void poly_invntt_tomont(poly *r);
static void poly_basemul_montgomery(poly *r, const poly *a, const poly *b);
static void poly_tomont(poly *r);

static void poly_reduce(poly *r);

static void poly_add(poly *r, const poly *a, const poly *b);
static void poly_sub(poly *r, const poly *a, const poly *b);

/*************** kyber/ref/polyvec.h */
typedef struct{
  poly vec[2];
} polyvec2;

typedef struct{
  poly vec[3];
} polyvec3;

typedef struct{
  poly vec[4];
} polyvec4;

static void polyvec_compress_2(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES2], const polyvec2 *a);
static void polyvec_compress_3(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES3], const polyvec3 *a);
static void polyvec_compress_4(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES4], const polyvec4 *a);
static void polyvec_decompress_2(polyvec2 *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES2]);
static void polyvec_decompress_3(polyvec3 *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES3]);
static void polyvec_decompress_4(polyvec4 *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES4]);

static void polyvec_tobytes_2(uint8_t r[KYBER_POLYVECBYTES2], const polyvec2 *a);
static void polyvec_tobytes_3(uint8_t r[KYBER_POLYVECBYTES3], const polyvec3 *a);
static void polyvec_tobytes_4(uint8_t r[KYBER_POLYVECBYTES4], const polyvec4 *a);
static void polyvec_frombytes_2(polyvec2 *r, const uint8_t a[KYBER_POLYVECBYTES2]);
static void polyvec_frombytes_3(polyvec3 *r, const uint8_t a[KYBER_POLYVECBYTES3]);
static void polyvec_frombytes_4(polyvec4 *r, const uint8_t a[KYBER_POLYVECBYTES4]);

static void polyvec_ntt_2(polyvec2 *r);
static void polyvec_ntt_3(polyvec3 *r);
static void polyvec_ntt_4(polyvec4 *r);
static void polyvec_invntt_tomont_2(polyvec2 *r);
static void polyvec_invntt_tomont_3(polyvec3 *r);
static void polyvec_invntt_tomont_4(polyvec4 *r);

static void polyvec_basemul_acc_montgomery_2(poly *r, const polyvec2 *a, const polyvec2 *b);
static void polyvec_basemul_acc_montgomery_3(poly *r, const polyvec3 *a, const polyvec3 *b);
static void polyvec_basemul_acc_montgomery_4(poly *r, const polyvec4 *a, const polyvec4 *b);

static void polyvec_reduce_2(polyvec2 *r);
static void polyvec_reduce_3(polyvec3 *r);
static void polyvec_reduce_4(polyvec4 *r);

static void polyvec_add_2(polyvec2 *r, const polyvec2 *a, const polyvec2 *b);
static void polyvec_add_3(polyvec3 *r, const polyvec3 *a, const polyvec3 *b);
static void polyvec_add_4(polyvec4 *r, const polyvec4 *a, const polyvec4 *b);

/*************** kyber/ref/indcpa.h */
static void indcpa_keypair_derand_2(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES2],
                                    uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES2],
                                    const uint8_t coins[KYBER_SYMBYTES]);
static void indcpa_keypair_derand_3(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES3],
                                    uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES3],
                                    const uint8_t coins[KYBER_SYMBYTES]);
static void indcpa_keypair_derand_4(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES4],
                                    uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES4],
                                    const uint8_t coins[KYBER_SYMBYTES]);

static void indcpa_enc_2(uint8_t c[KYBER_INDCPA_BYTES2],
                         const uint8_t m[KYBER_INDCPA_MSGBYTES],
                         const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES2],
                         const uint8_t coins[KYBER_SYMBYTES]);
static void indcpa_enc_3(uint8_t c[KYBER_INDCPA_BYTES3],
                         const uint8_t m[KYBER_INDCPA_MSGBYTES],
                         const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES3],
                         const uint8_t coins[KYBER_SYMBYTES]);
static void indcpa_enc_4(uint8_t c[KYBER_INDCPA_BYTES4],
                         const uint8_t m[KYBER_INDCPA_MSGBYTES],
                         const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES4],
                         const uint8_t coins[KYBER_SYMBYTES]);

static void indcpa_dec_2(uint8_t m[KYBER_INDCPA_MSGBYTES],
                         const uint8_t c[KYBER_INDCPA_BYTES2],
                         const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES2]);
static void indcpa_dec_3(uint8_t m[KYBER_INDCPA_MSGBYTES],
                         const uint8_t c[KYBER_INDCPA_BYTES3],
                         const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES3]);
static void indcpa_dec_4(uint8_t m[KYBER_INDCPA_MSGBYTES],
                         const uint8_t c[KYBER_INDCPA_BYTES4],
                         const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES4]);

/*************** kyber/ref/kem.h */
#define CRYPTO_SECRETKEYBYTES  KYBER_SECRETKEYBYTES
#define CRYPTO_PUBLICKEYBYTES  KYBER_PUBLICKEYBYTES
#define CRYPTO_CIPHERTEXTBYTES KYBER_CIPHERTEXTBYTES
#define CRYPTO_BYTES           KYBER_SSBYTES

static int crypto_kem_keypair_derand_2(uint8_t *pk, uint8_t *sk, const uint8_t *coins);
static int crypto_kem_keypair_derand_3(uint8_t *pk, uint8_t *sk, const uint8_t *coins);
static int crypto_kem_keypair_derand_4(uint8_t *pk, uint8_t *sk, const uint8_t *coins);

static int crypto_kem_enc_derand_2(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *coins);
static int crypto_kem_enc_derand_3(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *coins);
static int crypto_kem_enc_derand_4(uint8_t *ct, uint8_t *ss, const uint8_t *pk, const uint8_t *coins);

/*************** kyber/ref/ntt.h */
static const int16_t zetas[128];

static void ntt(int16_t poly[256]);

static void invntt(int16_t poly[256]);

static void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);

/*************** kyber/ref/randombytes.h */
void randombytes(uint8_t *out, size_t outlen);

/*************** kyber/ref/reduce.h */
#define MONT -1044 // 2^16 mod q
#define QINV -3327 // q^-1 mod 2^16

static int16_t montgomery_reduce(int32_t a);

static int16_t barrett_reduce(int16_t a);

/*************** kyber/ref/symmetric.h */
typedef keccak_state xof_state;

static void kyber_shake128_absorb(keccak_state *s,
                           const uint8_t seed[KYBER_SYMBYTES],
                           uint8_t x,
                           uint8_t y);

static void kyber_shake256_prf(uint8_t *out, size_t outlen, const uint8_t key[KYBER_SYMBYTES], uint8_t nonce);

static void kyber_shake256_rkprf2(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES2]);
static void kyber_shake256_rkprf3(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES3]);
static void kyber_shake256_rkprf4(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES4]);

#define XOF_BLOCKBYTES SHAKE128_RATE

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) kyber_shake128_absorb(STATE, SEED, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) kyber_shake256_prf(OUT, OUTBYTES, KEY, NONCE)
#define rkprf2(OUT, KEY, INPUT) kyber_shake256_rkprf2(OUT, KEY, INPUT)
#define rkprf3(OUT, KEY, INPUT) kyber_shake256_rkprf3(OUT, KEY, INPUT)
#define rkprf4(OUT, KEY, INPUT) kyber_shake256_rkprf4(OUT, KEY, INPUT)

/*************** kyber/ref/verify.h */
int verify(const uint8_t *a, const uint8_t *b, size_t len);

void cmov(uint8_t *r, const uint8_t *x, size_t len, uint8_t b);

/*************** kyber/ref/cbd.c */

/*************************************************
* Name:        load32_littleendian
*
* Description: load 4 bytes into a 32-bit integer
*              in little-endian order
*
* Arguments:   - const uint8_t *x: pointer to input byte array
*
* Returns 32-bit unsigned integer loaded from x
**************************************************/
static uint32_t load32_littleendian(const uint8_t x[4])
{
  uint32_t r;
  r  = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  r |= (uint32_t)x[3] << 24;
  return r;
}

/*************************************************
* Name:        load24_littleendian
*
* Description: load 3 bytes into a 32-bit integer
*              in little-endian order.
*              This function is only needed for Kyber-512
*
* Arguments:   - const uint8_t *x: pointer to input byte array
*
* Returns 32-bit unsigned integer loaded from x (most significant byte is zero)
**************************************************/
static uint32_t load24_littleendian(const uint8_t x[3])
{
  uint32_t r;
  r  = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  return r;
}


/*************************************************
* Name:        cbd2
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *buf: pointer to input byte array
**************************************************/
static void cbd2(poly *r, const uint8_t buf[2*KYBER_N/4])
{
  unsigned int i,j;
  uint32_t t,d;
  int16_t a,b;

  for(i=0;i<KYBER_N/8;i++) {
    t  = load32_littleendian(buf+4*i);
    d  = t & 0x55555555;
    d += (t>>1) & 0x55555555;

    for(j=0;j<8;j++) {
      a = (d >> (4*j+0)) & 0x3;
      b = (d >> (4*j+2)) & 0x3;
      r->coeffs[8*i+j] = a - b;
    }
  }
}

/*************************************************
* Name:        cbd3
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=3.
*              This function is only needed for Kyber-512
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *buf: pointer to input byte array
**************************************************/
static void cbd3(poly *r, const uint8_t buf[3*KYBER_N/4])
{
  unsigned int i,j;
  uint32_t t,d;
  int16_t a,b;

  for(i=0;i<KYBER_N/4;i++) {
    t  = load24_littleendian(buf+3*i);
    d  = t & 0x00249249;
    d += (t>>1) & 0x00249249;
    d += (t>>2) & 0x00249249;

    for(j=0;j<4;j++) {
      a = (d >> (6*j+0)) & 0x7;
      b = (d >> (6*j+3)) & 0x7;
      r->coeffs[4*i+j] = a - b;
    }
  }
}

/*************** kyber/ref/indcpa.c */

/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   uint8_t *r: pointer to the output serialized public key
*              polyvec *pk: pointer to the input public-key polyvec
*              const uint8_t *seed: pointer to the input public seed
**************************************************/
static void pack_pk_2(uint8_t r[KYBER_INDCPA_PUBLICKEYBYTES2],
                      polyvec2 *pk,
                      const uint8_t seed[KYBER_SYMBYTES])
{
  polyvec_tobytes_2(r, pk);
  memcpy(r+KYBER_POLYVECBYTES2, seed, KYBER_SYMBYTES);
}

static void pack_pk_3(uint8_t r[KYBER_INDCPA_PUBLICKEYBYTES3],
                      polyvec3 *pk,
                      const uint8_t seed[KYBER_SYMBYTES])
{
  polyvec_tobytes_3(r, pk);
  memcpy(r+KYBER_POLYVECBYTES3, seed, KYBER_SYMBYTES);
}

static void pack_pk_4(uint8_t r[KYBER_INDCPA_PUBLICKEYBYTES4],
                      polyvec4 *pk,
                      const uint8_t seed[KYBER_SYMBYTES])
{
  polyvec_tobytes_4(r, pk);
  memcpy(r+KYBER_POLYVECBYTES4, seed, KYBER_SYMBYTES);
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk: pointer to output public-key polynomial vector
*              - uint8_t *seed: pointer to output seed to generate matrix A
*              - const uint8_t *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk_2(polyvec2 *pk,
                        uint8_t seed[KYBER_SYMBYTES],
                        const uint8_t packedpk[KYBER_INDCPA_PUBLICKEYBYTES2])
{
  polyvec_frombytes_2(pk, packedpk);
  memcpy(seed, packedpk+KYBER_POLYVECBYTES2, KYBER_SYMBYTES);
}

static void unpack_pk_3(polyvec3 *pk,
                        uint8_t seed[KYBER_SYMBYTES],
                        const uint8_t packedpk[KYBER_INDCPA_PUBLICKEYBYTES3])
{
  polyvec_frombytes_3(pk, packedpk);
  memcpy(seed, packedpk+KYBER_POLYVECBYTES3, KYBER_SYMBYTES);
}

static void unpack_pk_4(polyvec4 *pk,
                        uint8_t seed[KYBER_SYMBYTES],
                        const uint8_t packedpk[KYBER_INDCPA_PUBLICKEYBYTES4])
{
  polyvec_frombytes_4(pk, packedpk);
  memcpy(seed, packedpk+KYBER_POLYVECBYTES4, KYBER_SYMBYTES);
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - uint8_t *r: pointer to output serialized secret key
*              - polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk_2(uint8_t r[KYBER_INDCPA_SECRETKEYBYTES2], polyvec2 *sk)
{
  polyvec_tobytes_2(r, sk);
}

static void pack_sk_3(uint8_t r[KYBER_INDCPA_SECRETKEYBYTES3], polyvec3 *sk)
{
  polyvec_tobytes_3(r, sk);
}

static void pack_sk_4(uint8_t r[KYBER_INDCPA_SECRETKEYBYTES4], polyvec4 *sk)
{
  polyvec_tobytes_4(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key; inverse of pack_sk
*
* Arguments:   - polyvec *sk: pointer to output vector of polynomials (secret key)
*              - const uint8_t *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk_2(polyvec2 *sk, const uint8_t packedsk[KYBER_INDCPA_SECRETKEYBYTES2])
{
  polyvec_frombytes_2(sk, packedsk);
}

static void unpack_sk_3(polyvec3 *sk, const uint8_t packedsk[KYBER_INDCPA_SECRETKEYBYTES3])
{
  polyvec_frombytes_3(sk, packedsk);
}

static void unpack_sk_4(polyvec4 *sk, const uint8_t packedsk[KYBER_INDCPA_SECRETKEYBYTES4])
{
  polyvec_frombytes_4(sk, packedsk);
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   uint8_t *r: pointer to the output serialized ciphertext
*              poly *pk: pointer to the input vector of polynomials b
*              poly *v: pointer to the input polynomial v
**************************************************/
static void pack_ciphertext_2(uint8_t r[KYBER_INDCPA_BYTES2], polyvec2 *b, poly *v)
{
  polyvec_compress_2(r, b);
  poly_compress_128(r+KYBER_POLYVECCOMPRESSEDBYTES2, v);
}

static void pack_ciphertext_3(uint8_t r[KYBER_INDCPA_BYTES3], polyvec3 *b, poly *v)
{
  polyvec_compress_3(r, b);
  poly_compress_128(r+KYBER_POLYVECCOMPRESSEDBYTES3, v);
}

static void pack_ciphertext_4(uint8_t r[KYBER_INDCPA_BYTES4], polyvec4 *b, poly *v)
{
  polyvec_compress_4(r, b);
  poly_compress_160(r+KYBER_POLYVECCOMPRESSEDBYTES4, v);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b: pointer to the output vector of polynomials b
*              - poly *v: pointer to the output polynomial v
*              - const uint8_t *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext_2(polyvec2 *b, poly *v, const uint8_t c[KYBER_INDCPA_BYTES2])
{
  polyvec_decompress_2(b, c);
  poly_decompress_128(v, c+KYBER_POLYVECCOMPRESSEDBYTES2);
}

static void unpack_ciphertext_3(polyvec3 *b, poly *v, const uint8_t c[KYBER_INDCPA_BYTES3])
{
  polyvec_decompress_3(b, c);
  poly_decompress_128(v, c+KYBER_POLYVECCOMPRESSEDBYTES3);
}

static void unpack_ciphertext_4(polyvec4 *b, poly *v, const uint8_t c[KYBER_INDCPA_BYTES4])
{
  polyvec_decompress_4(b, c);
  poly_decompress_160(v, c+KYBER_POLYVECCOMPRESSEDBYTES4);
}

/*************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   - int16_t *r: pointer to output buffer
*              - unsigned int len: requested number of 16-bit integers (uniform mod q)
*              - const uint8_t *buf: pointer to input buffer (assumed to be uniformly random bytes)
*              - unsigned int buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
**************************************************/
static unsigned int rej_uniform(int16_t *r,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen)
{
  unsigned int ctr, pos;
  uint16_t val0, val1;

  ctr = pos = 0;
  while(ctr < len && pos + 3 <= buflen) {
    val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
    val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
    pos += 3;

    if(val0 < KYBER_Q)
      r[ctr++] = val0;
    if(ctr < len && val1 < KYBER_Q)
      r[ctr++] = val1;
  }

  return ctr;
}

#define gen_a_2(A,B)  gen_matrix_2(A,B,0)
#define gen_at_2(A,B) gen_matrix_2(A,B,1)
#define gen_a_3(A,B)  gen_matrix_3(A,B,0)
#define gen_at_3(A,B) gen_matrix_3(A,B,1)
#define gen_a_4(A,B)  gen_matrix_4(A,B,0)
#define gen_at_4(A,B) gen_matrix_4(A,B,1)

/*************************************************
* Name:        gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   - polyvec *a: pointer to ouptput matrix A
*              - const uint8_t *seed: pointer to input seed
*              - int transposed: boolean deciding whether A or A^T is generated
**************************************************/
#define GEN_MATRIX_NBLOCKS ((12*KYBER_N/8*(1 << 12)/KYBER_Q + XOF_BLOCKBYTES)/XOF_BLOCKBYTES)

#define KYBER_K 2
static void gen_matrix_2(polyvec2 *a, const uint8_t seed[KYBER_SYMBYTES], int transposed)
{
  unsigned int ctr, i, j, k;
  unsigned int buflen, off;
  uint8_t buf[GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+2];
  xof_state state;

  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_K;j++) {
      if(transposed)
        xof_absorb(&state, seed, i, j);
      else
        xof_absorb(&state, seed, j, i);

      xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
      buflen = GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES;
      ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

      while(ctr < KYBER_N) {
        off = buflen % 3;
        for(k = 0; k < off; k++)
          buf[k] = buf[buflen - off + k];
        xof_squeezeblocks(buf + off, 1, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, buflen);
      }
    }
  }
}
#undef KYBER_K

#define KYBER_K 3
static void gen_matrix_3(polyvec3 *a, const uint8_t seed[KYBER_SYMBYTES], int transposed)
{
  unsigned int ctr, i, j, k;
  unsigned int buflen, off;
  uint8_t buf[GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+2];
  xof_state state;

  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_K;j++) {
      if(transposed)
        xof_absorb(&state, seed, i, j);
      else
        xof_absorb(&state, seed, j, i);

      xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
      buflen = GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES;
      ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

      while(ctr < KYBER_N) {
        off = buflen % 3;
        for(k = 0; k < off; k++)
          buf[k] = buf[buflen - off + k];
        xof_squeezeblocks(buf + off, 1, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, buflen);
      }
    }
  }
}
#undef KYBER_K

#define KYBER_K 4
static void gen_matrix_4(polyvec4 *a, const uint8_t seed[KYBER_SYMBYTES], int transposed)
{
  unsigned int ctr, i, j, k;
  unsigned int buflen, off;
  uint8_t buf[GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+2];
  xof_state state;

  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_K;j++) {
      if(transposed)
        xof_absorb(&state, seed, i, j);
      else
        xof_absorb(&state, seed, j, i);

      xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
      buflen = GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES;
      ctr = rej_uniform(a[i].vec[j].coeffs, KYBER_N, buf, buflen);

      while(ctr < KYBER_N) {
        off = buflen % 3;
        for(k = 0; k < off; k++)
          buf[k] = buf[buflen - off + k];
        xof_squeezeblocks(buf + off, 1, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, KYBER_N - ctr, buf, buflen);
      }
    }
  }
}
#undef KYBER_K

/*************************************************
* Name:        indcpa_keypair_derand
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                             (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
*                             (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
*              - const uint8_t *coins: pointer to input randomness
*                             (of length KYBER_SYMBYTES bytes)
**************************************************/
#define KYBER_K 2
void indcpa_keypair_derand_2(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES2],
                             uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES2],
                             const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t buf[2*KYBER_SYMBYTES];
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf+KYBER_SYMBYTES;
  uint8_t nonce = 0;
  polyvec2 a[KYBER_K], e, pkpv, skpv;

  hash_g(buf, coins, KYBER_SYMBYTES);

  gen_a_2(a, publicseed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_2(&skpv.vec[i], noiseseed, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_2(&e.vec[i], noiseseed, nonce++);

  polyvec_ntt_2(&skpv);
  polyvec_ntt_2(&e);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++) {
    polyvec_basemul_acc_montgomery_2(&pkpv.vec[i], &a[i], &skpv);
    poly_tomont(&pkpv.vec[i]);
  }

  polyvec_add_2(&pkpv, &pkpv, &e);
  polyvec_reduce_2(&pkpv);

  pack_sk_2(sk, &skpv);
  pack_pk_2(pk, &pkpv, publicseed);
}
#undef KYBER_K

#define KYBER_K 3
void indcpa_keypair_derand_3(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES3],
                             uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES3],
                             const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t buf[2*KYBER_SYMBYTES];
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf+KYBER_SYMBYTES;
  uint8_t nonce = 0;
  polyvec3 a[KYBER_K], e, pkpv, skpv;

  hash_g(buf, coins, KYBER_SYMBYTES);

  gen_a_3(a, publicseed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_3_4(&skpv.vec[i], noiseseed, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_3_4(&e.vec[i], noiseseed, nonce++);

  polyvec_ntt_3(&skpv);
  polyvec_ntt_3(&e);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++) {
    polyvec_basemul_acc_montgomery_3(&pkpv.vec[i], &a[i], &skpv);
    poly_tomont(&pkpv.vec[i]);
  }

  polyvec_add_3(&pkpv, &pkpv, &e);
  polyvec_reduce_3(&pkpv);

  pack_sk_3(sk, &skpv);
  pack_pk_3(pk, &pkpv, publicseed);
}
#undef KYBER_K

#define KYBER_K 4
void indcpa_keypair_derand_4(uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES4],
                             uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES4],
                             const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t buf[2*KYBER_SYMBYTES];
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf+KYBER_SYMBYTES;
  uint8_t nonce = 0;
  polyvec4 a[KYBER_K], e, pkpv, skpv;

  hash_g(buf, coins, KYBER_SYMBYTES);

  gen_a_4(a, publicseed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_3_4(&skpv.vec[i], noiseseed, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_3_4(&e.vec[i], noiseseed, nonce++);

  polyvec_ntt_4(&skpv);
  polyvec_ntt_4(&e);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++) {
    polyvec_basemul_acc_montgomery_4(&pkpv.vec[i], &a[i], &skpv);
    poly_tomont(&pkpv.vec[i]);
  }

  polyvec_add_4(&pkpv, &pkpv, &e);
  polyvec_reduce_4(&pkpv);

  pack_sk_4(sk, &skpv);
  pack_pk_4(pk, &pkpv, publicseed);
}
#undef KYBER_K

/*************************************************
* Name:        indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *c: pointer to output ciphertext
*                            (of length KYBER_INDCPA_BYTES bytes)
*              - const uint8_t *m: pointer to input message
*                                  (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                                   (of length KYBER_INDCPA_PUBLICKEYBYTES)
*              - const uint8_t *coins: pointer to input random coins used as seed
*                                      (of length KYBER_SYMBYTES) to deterministically
*                                      generate all randomness
**************************************************/
#define KYBER_K 2
void indcpa_enc_2(uint8_t c[KYBER_INDCPA_BYTES2],
                  const uint8_t m[KYBER_INDCPA_MSGBYTES],
                  const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES2],
                  const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t seed[KYBER_SYMBYTES];
  uint8_t nonce = 0;
  polyvec2 sp, pkpv, ep, at[KYBER_K], b;
  poly v, k, epp;

  unpack_pk_2(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_at_2(at, seed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_2(sp.vec+i, coins, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta2(ep.vec+i, coins, nonce++);
  poly_getnoise_eta2(&epp, coins, nonce++);

  polyvec_ntt_2(&sp);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++)
    polyvec_basemul_acc_montgomery_2(&b.vec[i], &at[i], &sp);

  polyvec_basemul_acc_montgomery_2(&v, &pkpv, &sp);

  polyvec_invntt_tomont_2(&b);
  poly_invntt_tomont(&v);

  polyvec_add_2(&b, &b, &ep);
  poly_add(&v, &v, &epp);
  poly_add(&v, &v, &k);
  polyvec_reduce_2(&b);
  poly_reduce(&v);

  pack_ciphertext_2(c, &b, &v);
}
#undef KYBER_K

#define KYBER_K 3
void indcpa_enc_3(uint8_t c[KYBER_INDCPA_BYTES3],
                  const uint8_t m[KYBER_INDCPA_MSGBYTES],
                  const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES3],
                  const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t seed[KYBER_SYMBYTES];
  uint8_t nonce = 0;
  polyvec3 sp, pkpv, ep, at[KYBER_K], b;
  poly v, k, epp;

  unpack_pk_3(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_at_3(at, seed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_3_4(sp.vec+i, coins, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta2(ep.vec+i, coins, nonce++);
  poly_getnoise_eta2(&epp, coins, nonce++);

  polyvec_ntt_3(&sp);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++)
    polyvec_basemul_acc_montgomery_3(&b.vec[i], &at[i], &sp);

  polyvec_basemul_acc_montgomery_3(&v, &pkpv, &sp);

  polyvec_invntt_tomont_3(&b);
  poly_invntt_tomont(&v);

  polyvec_add_3(&b, &b, &ep);
  poly_add(&v, &v, &epp);
  poly_add(&v, &v, &k);
  polyvec_reduce_3(&b);
  poly_reduce(&v);

  pack_ciphertext_3(c, &b, &v);
}
#undef KYBER_K

#define KYBER_K 4
void indcpa_enc_4(uint8_t c[KYBER_INDCPA_BYTES4],
                  const uint8_t m[KYBER_INDCPA_MSGBYTES],
                  const uint8_t pk[KYBER_INDCPA_PUBLICKEYBYTES4],
                  const uint8_t coins[KYBER_SYMBYTES])
{
  unsigned int i;
  uint8_t seed[KYBER_SYMBYTES];
  uint8_t nonce = 0;
  polyvec4 sp, pkpv, ep, at[KYBER_K], b;
  poly v, k, epp;

  unpack_pk_4(&pkpv, seed, pk);
  poly_frommsg(&k, m);
  gen_at_4(at, seed);

  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta1_3_4(sp.vec+i, coins, nonce++);
  for(i=0;i<KYBER_K;i++)
    poly_getnoise_eta2(ep.vec+i, coins, nonce++);
  poly_getnoise_eta2(&epp, coins, nonce++);

  polyvec_ntt_4(&sp);

  // matrix-vector multiplication
  for(i=0;i<KYBER_K;i++)
    polyvec_basemul_acc_montgomery_4(&b.vec[i], &at[i], &sp);

  polyvec_basemul_acc_montgomery_4(&v, &pkpv, &sp);

  polyvec_invntt_tomont_4(&b);
  poly_invntt_tomont(&v);

  polyvec_add_4(&b, &b, &ep);
  poly_add(&v, &v, &epp);
  poly_add(&v, &v, &k);
  polyvec_reduce_4(&b);
  poly_reduce(&v);

  pack_ciphertext_4(c, &b, &v);
}
#undef KYBER_K

/*************************************************
* Name:        indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *m: pointer to output decrypted message
*                            (of length KYBER_INDCPA_MSGBYTES)
*              - const uint8_t *c: pointer to input ciphertext
*                                  (of length KYBER_INDCPA_BYTES)
*              - const uint8_t *sk: pointer to input secret key
*                                   (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void indcpa_dec_2(uint8_t m[KYBER_INDCPA_MSGBYTES],
                  const uint8_t c[KYBER_INDCPA_BYTES2],
                  const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES2])
{
  polyvec2 b, skpv;
  poly v, mp;

  unpack_ciphertext_2(&b, &v, c);
  unpack_sk_2(&skpv, sk);

  polyvec_ntt_2(&b);
  polyvec_basemul_acc_montgomery_2(&mp, &skpv, &b);
  poly_invntt_tomont(&mp);

  poly_sub(&mp, &v, &mp);
  poly_reduce(&mp);

  poly_tomsg(m, &mp);
}

void indcpa_dec_3(uint8_t m[KYBER_INDCPA_MSGBYTES],
                  const uint8_t c[KYBER_INDCPA_BYTES3],
                  const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES3])
{
  polyvec3 b, skpv;
  poly v, mp;

  unpack_ciphertext_3(&b, &v, c);
  unpack_sk_3(&skpv, sk);

  polyvec_ntt_3(&b);
  polyvec_basemul_acc_montgomery_3(&mp, &skpv, &b);
  poly_invntt_tomont(&mp);

  poly_sub(&mp, &v, &mp);
  poly_reduce(&mp);

  poly_tomsg(m, &mp);
}

void indcpa_dec_4(uint8_t m[KYBER_INDCPA_MSGBYTES],
                  const uint8_t c[KYBER_INDCPA_BYTES4],
                  const uint8_t sk[KYBER_INDCPA_SECRETKEYBYTES4])
{
  polyvec4 b, skpv;
  poly v, mp;

  unpack_ciphertext_4(&b, &v, c);
  unpack_sk_4(&skpv, sk);

  polyvec_ntt_4(&b);
  polyvec_basemul_acc_montgomery_4(&mp, &skpv, &b);
  poly_invntt_tomont(&mp);

  poly_sub(&mp, &v, &mp);
  poly_reduce(&mp);

  poly_tomsg(m, &mp);
}
/*************** kyber/ref/kem.c */
/*************************************************
* Name:        crypto_kem_keypair_derand
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*              - uint8_t *coins: pointer to input randomness
*                (an already allocated array filled with 2*KYBER_SYMBYTES random bytes)
**
* Returns 0 (success)
**************************************************/
int crypto_kem_keypair_derand_2(uint8_t *pk,
                                uint8_t *sk,
                                const uint8_t *coins)
{
  indcpa_keypair_derand_2(pk, sk, coins);
  memcpy(sk+KYBER_INDCPA_SECRETKEYBYTES2, pk, KYBER_PUBLICKEYBYTES2);
  hash_h(sk+KYBER_SECRETKEYBYTES2-2*KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES2);
  /* Value z for pseudo-random output on reject */
  memcpy(sk+KYBER_SECRETKEYBYTES2-KYBER_SYMBYTES, coins+KYBER_SYMBYTES, KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_keypair_derand_3(uint8_t *pk,
                                uint8_t *sk,
                                const uint8_t *coins)
{
  indcpa_keypair_derand_3(pk, sk, coins);
  memcpy(sk+KYBER_INDCPA_SECRETKEYBYTES3, pk, KYBER_PUBLICKEYBYTES3);
  hash_h(sk+KYBER_SECRETKEYBYTES3-2*KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES3);
  /* Value z for pseudo-random output on reject */
  memcpy(sk+KYBER_SECRETKEYBYTES3-KYBER_SYMBYTES, coins+KYBER_SYMBYTES, KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_keypair_derand_4(uint8_t *pk,
                                uint8_t *sk,
                                const uint8_t *coins)
{
  indcpa_keypair_derand_4(pk, sk, coins);
  memcpy(sk+KYBER_INDCPA_SECRETKEYBYTES4, pk, KYBER_PUBLICKEYBYTES4);
  hash_h(sk+KYBER_SECRETKEYBYTES4-2*KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES4);
  /* Value z for pseudo-random output on reject */
  memcpy(sk+KYBER_SECRETKEYBYTES4-KYBER_SYMBYTES, coins+KYBER_SYMBYTES, KYBER_SYMBYTES);
  return 0;
}

/*************************************************
* Name:        crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_kem_keypair_512(uint8_t *pk, uint8_t *sk)
{
  uint8_t coins[2*KYBER_SYMBYTES];
  randombytes(coins, 2*KYBER_SYMBYTES);
  crypto_kem_keypair_derand_2(pk, sk, coins);
  return 0;
}

int crypto_kem_keypair_768(uint8_t *pk, uint8_t *sk)
{
  uint8_t coins[2*KYBER_SYMBYTES];
  randombytes(coins, 2*KYBER_SYMBYTES);
  crypto_kem_keypair_derand_3(pk, sk, coins);
  return 0;
}

int crypto_kem_keypair_1024(uint8_t *pk, uint8_t *sk)
{
  uint8_t coins[2*KYBER_SYMBYTES];
  randombytes(coins, 2*KYBER_SYMBYTES);
  crypto_kem_keypair_derand_4(pk, sk, coins);
  return 0;
}

/*************************************************
* Name:        crypto_kem_enc_derand
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - uint8_t *ct: pointer to output cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*              - const uint8_t *coins: pointer to input randomness
*                (an already allocated array filled with KYBER_SYMBYTES random bytes)
**
* Returns 0 (success)
**************************************************/
int crypto_kem_enc_derand_2(uint8_t *ct,
                            uint8_t *ss,
                            const uint8_t *pk,
                            const uint8_t *coins)
{
  uint8_t buf[2*KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*KYBER_SYMBYTES];

  memcpy(buf, coins, KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  hash_h(buf+KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES2);
  hash_g(kr, buf, 2*KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc_2(ct, buf, pk, kr+KYBER_SYMBYTES);

  memcpy(ss,kr,KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_enc_derand_3(uint8_t *ct,
                            uint8_t *ss,
                            const uint8_t *pk,
                            const uint8_t *coins)
{
  uint8_t buf[2*KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*KYBER_SYMBYTES];

  memcpy(buf, coins, KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  hash_h(buf+KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES3);
  hash_g(kr, buf, 2*KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc_3(ct, buf, pk, kr+KYBER_SYMBYTES);

  memcpy(ss,kr,KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_enc_derand_4(uint8_t *ct,
                            uint8_t *ss,
                            const uint8_t *pk,
                            const uint8_t *coins)
{
  uint8_t buf[2*KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*KYBER_SYMBYTES];

  memcpy(buf, coins, KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  hash_h(buf+KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES4);
  hash_g(kr, buf, 2*KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc_4(ct, buf, pk, kr+KYBER_SYMBYTES);

  memcpy(ss,kr,KYBER_SYMBYTES);
  return 0;
}

/*************************************************
* Name:        crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - uint8_t *ct: pointer to output cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_kem_enc_512(uint8_t *ct,
                       uint8_t *ss,
                       const uint8_t *pk)
{
  uint8_t coins[KYBER_SYMBYTES];
  randombytes(coins, KYBER_SYMBYTES);
  crypto_kem_enc_derand_2(ct, ss, pk, coins);
  return 0;
}

int crypto_kem_enc_768(uint8_t *ct,
                       uint8_t *ss,
                       const uint8_t *pk)
{
  uint8_t coins[KYBER_SYMBYTES];
  randombytes(coins, KYBER_SYMBYTES);
  crypto_kem_enc_derand_3(ct, ss, pk, coins);
  return 0;
}

int crypto_kem_enc_1024(uint8_t *ct,
                        uint8_t *ss,
                        const uint8_t *pk)
{
  uint8_t coins[KYBER_SYMBYTES];
  randombytes(coins, KYBER_SYMBYTES);
  crypto_kem_enc_derand_4(ct, ss, pk, coins);
  return 0;
}

/*************************************************
* Name:        crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *ct: pointer to input cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - const uint8_t *sk: pointer to input private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
**************************************************/
int crypto_kem_dec_512(uint8_t *ss,
                       const uint8_t *ct,
                       const uint8_t *sk)
{
  int fail;
  uint8_t buf[2*KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*KYBER_SYMBYTES];
  uint8_t cmp[KYBER_CIPHERTEXTBYTES2+KYBER_SYMBYTES];
  const uint8_t *pk = sk+KYBER_INDCPA_SECRETKEYBYTES2;

  indcpa_dec_2(buf, ct, sk);

  /* Multitarget countermeasure for coins + contributory KEM */
  memcpy(buf+KYBER_SYMBYTES, sk+KYBER_SECRETKEYBYTES2-2*KYBER_SYMBYTES, KYBER_SYMBYTES);
  hash_g(kr, buf, 2*KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc_2(cmp, buf, pk, kr+KYBER_SYMBYTES);

  fail = verify(ct, cmp, KYBER_CIPHERTEXTBYTES2);

  /* Compute rejection key */
  rkprf2(ss,sk+KYBER_SECRETKEYBYTES2-KYBER_SYMBYTES,ct);

  /* Copy true key to return buffer if fail is false */
  cmov(ss,kr,KYBER_SYMBYTES,!fail);

  return 0;
}

int crypto_kem_dec_768(uint8_t *ss,
                       const uint8_t *ct,
                       const uint8_t *sk)
{
  int fail;
  uint8_t buf[2*KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*KYBER_SYMBYTES];
  uint8_t cmp[KYBER_CIPHERTEXTBYTES3+KYBER_SYMBYTES];
  const uint8_t *pk = sk+KYBER_INDCPA_SECRETKEYBYTES3;

  indcpa_dec_3(buf, ct, sk);

  /* Multitarget countermeasure for coins + contributory KEM */
  memcpy(buf+KYBER_SYMBYTES, sk+KYBER_SECRETKEYBYTES3-2*KYBER_SYMBYTES, KYBER_SYMBYTES);
  hash_g(kr, buf, 2*KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc_3(cmp, buf, pk, kr+KYBER_SYMBYTES);

  fail = verify(ct, cmp, KYBER_CIPHERTEXTBYTES3);

  /* Compute rejection key */
  rkprf3(ss,sk+KYBER_SECRETKEYBYTES3-KYBER_SYMBYTES,ct);

  /* Copy true key to return buffer if fail is false */
  cmov(ss,kr,KYBER_SYMBYTES,!fail);

  return 0;
}

int crypto_kem_dec_1024(uint8_t *ss,
                        const uint8_t *ct,
                        const uint8_t *sk)
{
  int fail;
  uint8_t buf[2*KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*KYBER_SYMBYTES];
  uint8_t cmp[KYBER_CIPHERTEXTBYTES4+KYBER_SYMBYTES];
  const uint8_t *pk = sk+KYBER_INDCPA_SECRETKEYBYTES4;

  indcpa_dec_4(buf, ct, sk);

  /* Multitarget countermeasure for coins + contributory KEM */
  memcpy(buf+KYBER_SYMBYTES, sk+KYBER_SECRETKEYBYTES4-2*KYBER_SYMBYTES, KYBER_SYMBYTES);
  hash_g(kr, buf, 2*KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc_4(cmp, buf, pk, kr+KYBER_SYMBYTES);

  fail = verify(ct, cmp, KYBER_CIPHERTEXTBYTES4);

  /* Compute rejection key */
  rkprf4(ss,sk+KYBER_SECRETKEYBYTES4-KYBER_SYMBYTES,ct);

  /* Copy true key to return buffer if fail is false */
  cmov(ss,kr,KYBER_SYMBYTES,!fail);

  return 0;
}
/*************** kyber/ref/ntt.c */
/* Code to generate zetas and zetas_inv used in the number-theoretic transform:

#define KYBER_ROOT_OF_UNITY 17

static const uint8_t tree[128] = {
  0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120,
  4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124,
  2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122,
  6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126,
  1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121,
  5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125,
  3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123,
  7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127
};

void init_ntt() {
  unsigned int i;
  int16_t tmp[128];

  tmp[0] = MONT;
  for(i=1;i<128;i++)
    tmp[i] = fqmul(tmp[i-1],MONT*KYBER_ROOT_OF_UNITY % KYBER_Q);

  for(i=0;i<128;i++) {
    zetas[i] = tmp[tree[i]];
    if(zetas[i] > KYBER_Q/2)
      zetas[i] -= KYBER_Q;
    if(zetas[i] < -KYBER_Q/2)
      zetas[i] += KYBER_Q;
  }
}
*/

static const int16_t zetas[128] = {
  -1044,  -758,  -359, -1517,  1493,  1422,   287,   202,
   -171,   622,  1577,   182,   962, -1202, -1474,  1468,
    573, -1325,   264,   383,  -829,  1458, -1602,  -130,
   -681,  1017,   732,   608, -1542,   411,  -205, -1571,
   1223,   652,  -552,  1015, -1293,  1491,  -282, -1544,
    516,    -8,  -320,  -666, -1618, -1162,   126,  1469,
   -853,   -90,  -271,   830,   107, -1421,  -247,  -951,
   -398,   961, -1508,  -725,   448, -1065,   677, -1275,
  -1103,   430,   555,   843, -1251,   871,  1550,   105,
    422,   587,   177,  -235,  -291,  -460,  1574,  1653,
   -246,   778,  1159,  -147,  -777,  1483,  -602,  1119,
  -1590,   644,  -872,   349,   418,   329,  -156,   -75,
    817,  1097,   603,   610,  1322, -1285, -1465,   384,
  -1215,  -136,  1218, -1335,  -874,   220, -1187, -1659,
  -1185, -1530, -1278,   794, -1510,  -854,  -870,   478,
   -108,  -308,   996,   991,   958, -1460,  1522,  1628
};

/*************************************************
* Name:        fqmul
*
* Description: Multiplication followed by Montgomery reduction
*
* Arguments:   - int16_t a: first factor
*              - int16_t b: second factor
*
* Returns 16-bit integer congruent to a*b*R^{-1} mod q
**************************************************/
static int16_t fqmul(int16_t a, int16_t b) {
  return montgomery_reduce((int32_t)a*b);
}

/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq.
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void ntt(int16_t r[256]) {
  unsigned int len, start, j, k;
  int16_t t, zeta;

  k = 1;
  for(len = 128; len >= 2; len >>= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k++];
      for(j = start; j < start + len; j++) {
        t = fqmul(zeta, r[j + len]);
        r[j + len] = r[j] - t;
        r[j] = r[j] + t;
      }
    }
  }
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inplace inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor 2^16.
*              Input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
void invntt(int16_t r[256]) {
  unsigned int start, len, j, k;
  int16_t t, zeta;
  const int16_t f = 1441; // mont^2/128

  k = 127;
  for(len = 2; len <= 128; len <<= 1) {
    for(start = 0; start < 256; start = j + len) {
      zeta = zetas[k--];
      for(j = start; j < start + len; j++) {
        t = r[j];
        r[j] = barrett_reduce(t + r[j + len]);
        r[j + len] = r[j + len] - t;
        r[j + len] = fqmul(zeta, r[j + len]);
      }
    }
  }

  for(j = 0; j < 256; j++)
    r[j] = fqmul(r[j], f);
}

/*************************************************
* Name:        basemul
*
* Description: Multiplication of polynomials in Zq[X]/(X^2-zeta)
*              used for multiplication of elements in Rq in NTT domain
*
* Arguments:   - int16_t r[2]: pointer to the output polynomial
*              - const int16_t a[2]: pointer to the first factor
*              - const int16_t b[2]: pointer to the second factor
*              - int16_t zeta: integer defining the reduction polynomial
**************************************************/
void basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta)
{
  r[0]  = fqmul(a[1], b[1]);
  r[0]  = fqmul(r[0], zeta);
  r[0] += fqmul(a[0], b[0]);
  r[1]  = fqmul(a[0], b[1]);
  r[1] += fqmul(a[1], b[0]);
}
/*************** kyber/ref/poly.c */

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_compress_128(uint8_t r[KYBER_POLYCOMPRESSEDBYTES2], const poly *a)
{
  unsigned int i,j;
  int32_t u;
  uint32_t d0;
  uint8_t t[8];

  for(i=0;i<KYBER_N/8;i++) {
    for(j=0;j<8;j++) {
      // map to positive standard representatives
      u  = a->coeffs[8*i+j];
      u += (u >> 15) & KYBER_Q;
/*    t[j] = ((((uint16_t)u << 4) + KYBER_Q/2)/KYBER_Q) & 15; */
      d0 = u << 4;
      d0 += 1665;
      d0 *= 80635;
      d0 >>= 28;
      t[j] = d0 & 0xf;
    }

    r[0] = t[0] | (t[1] << 4);
    r[1] = t[2] | (t[3] << 4);
    r[2] = t[4] | (t[5] << 4);
    r[3] = t[6] | (t[7] << 4);
    r += 4;
  }
}

void poly_compress_160(uint8_t r[KYBER_POLYCOMPRESSEDBYTES4], const poly *a)
{
  unsigned int i,j;
  int32_t u;
  uint32_t d0;
  uint8_t t[8];

  for(i=0;i<KYBER_N/8;i++) {
    for(j=0;j<8;j++) {
      // map to positive standard representatives
      u  = a->coeffs[8*i+j];
      u += (u >> 15) & KYBER_Q;
/*    t[j] = ((((uint32_t)u << 5) + KYBER_Q/2)/KYBER_Q) & 31; */
      d0 = u << 5;
      d0 += 1664;
      d0 *= 40318;
      d0 >>= 27;
      t[j] = d0 & 0x1f;
    }

    r[0] = (t[0] >> 0) | (t[1] << 5);
    r[1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
    r[2] = (t[3] >> 1) | (t[4] << 4);
    r[3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
    r[4] = (t[6] >> 2) | (t[7] << 3);
    r += 5;
  }
}

/*************************************************
* Name:        poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of poly_compress
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void poly_decompress_128(poly *r, const uint8_t a[KYBER_POLYCOMPRESSEDBYTES2])
{
  unsigned int i;
  for(i=0;i<KYBER_N/2;i++) {
    r->coeffs[2*i+0] = (((uint16_t)(a[0] & 15)*KYBER_Q) + 8) >> 4;
    r->coeffs[2*i+1] = (((uint16_t)(a[0] >> 4)*KYBER_Q) + 8) >> 4;
    a += 1;
  }
}

void poly_decompress_160(poly *r, const uint8_t a[KYBER_POLYCOMPRESSEDBYTES4])
{
  unsigned int i;
  unsigned int j;
  uint8_t t[8];
  for(i=0;i<KYBER_N/8;i++) {
    t[0] = (a[0] >> 0);
    t[1] = (a[0] >> 5) | (a[1] << 3);
    t[2] = (a[1] >> 2);
    t[3] = (a[1] >> 7) | (a[2] << 1);
    t[4] = (a[2] >> 4) | (a[3] << 4);
    t[5] = (a[3] >> 1);
    t[6] = (a[3] >> 6) | (a[4] << 2);
    t[7] = (a[4] >> 3);
    a += 5;

    for(j=0;j<8;j++)
      r->coeffs[8*i+j] = ((uint32_t)(t[j] & 31)*KYBER_Q + 16) >> 5;
  }
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tobytes(uint8_t r[KYBER_POLYBYTES], const poly *a)
{
  unsigned int i;
  uint16_t t0, t1;

  for(i=0;i<KYBER_N/2;i++) {
    // map to positive standard representatives
    t0  = a->coeffs[2*i];
    t0 += ((int16_t)t0 >> 15) & KYBER_Q;
    t1 = a->coeffs[2*i+1];
    t1 += ((int16_t)t1 >> 15) & KYBER_Q;
    r[3*i+0] = (t0 >> 0);
    r[3*i+1] = (t0 >> 8) | (t1 << 4);
    r[3*i+2] = (t1 >> 4);
  }
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void poly_frombytes(poly *r, const uint8_t a[KYBER_POLYBYTES])
{
  unsigned int i;
  for(i=0;i<KYBER_N/2;i++) {
    r->coeffs[2*i]   = ((a[3*i+0] >> 0) | ((uint16_t)a[3*i+1] << 8)) & 0xFFF;
    r->coeffs[2*i+1] = ((a[3*i+1] >> 4) | ((uint16_t)a[3*i+2] << 4)) & 0xFFF;
  }
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void poly_frommsg(poly *r, const uint8_t msg[KYBER_INDCPA_MSGBYTES])
{
  unsigned int i,j;
  int16_t mask;

#if (KYBER_INDCPA_MSGBYTES != KYBER_N/8)
#error "KYBER_INDCPA_MSGBYTES must be equal to KYBER_N/8 bytes!"
#endif

  for(i=0;i<KYBER_N/8;i++) {
    for(j=0;j<8;j++) {
      mask = -(int16_t)((msg[i] >> j)&1);
      r->coeffs[8*i+j] = mask & ((KYBER_Q+1)/2);
    }
  }
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], const poly *a)
{
  unsigned int i,j;
  uint32_t t;

  for(i=0;i<KYBER_N/8;i++) {
    msg[i] = 0;
    for(j=0;j<8;j++) {
      t  = a->coeffs[8*i+j];
      // t += ((int16_t)t >> 15) & KYBER_Q;
      // t  = (((t << 1) + KYBER_Q/2)/KYBER_Q) & 1;
      t <<= 1;
      t += 1665;
      t *= 80635;
      t >>= 28;
      t &= 1;
      msg[i] |= t << j;
    }
  }
}

/*************************************************
* Name:        poly_getnoise_eta1
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA1
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta1_2(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce)
{
  uint8_t buf[KYBER_ETA1_2*KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce);
  cbd3(r, buf);
}

void poly_getnoise_eta1_3_4(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce)
{
  uint8_t buf[KYBER_ETA1_3_4*KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce);
  cbd2(r, buf);
}

/*************************************************
* Name:        poly_getnoise_eta2
*
* Description: Sample a polynomial deterministically from a seed and a nonce,
*              with output polynomial close to centered binomial distribution
*              with parameter KYBER_ETA2
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *seed: pointer to input seed
*                                     (of length KYBER_SYMBYTES bytes)
*              - uint8_t nonce: one-byte input nonce
**************************************************/
void poly_getnoise_eta2(poly *r, const uint8_t seed[KYBER_SYMBYTES], uint8_t nonce)
{
  uint8_t buf[KYBER_ETA2*KYBER_N/4];
  prf(buf, sizeof(buf), seed, nonce);
  cbd2(r, buf);
}


/*************************************************
* Name:        poly_ntt
*
* Description: Computes negacyclic number-theoretic transform (NTT) of
*              a polynomial in place;
*              inputs assumed to be in normal order, output in bitreversed order
*
* Arguments:   - uint16_t *r: pointer to in/output polynomial
**************************************************/
void poly_ntt(poly *r)
{
  ntt(r->coeffs);
  poly_reduce(r);
}

/*************************************************
* Name:        poly_invntt_tomont
*
* Description: Computes inverse of negacyclic number-theoretic transform (NTT)
*              of a polynomial in place;
*              inputs assumed to be in bitreversed order, output in normal order
*
* Arguments:   - uint16_t *a: pointer to in/output polynomial
**************************************************/
void poly_invntt_tomont(poly *r)
{
  invntt(r->coeffs);
}

/*************************************************
* Name:        poly_basemul_montgomery
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_basemul_montgomery(poly *r, const poly *a, const poly *b)
{
  unsigned int i;
  for(i=0;i<KYBER_N/4;i++) {
    basemul(&r->coeffs[4*i], &a->coeffs[4*i], &b->coeffs[4*i], zetas[64+i]);
    basemul(&r->coeffs[4*i+2], &a->coeffs[4*i+2], &b->coeffs[4*i+2], -zetas[64+i]);
  }
}

/*************************************************
* Name:        poly_tomont
*
* Description: Inplace conversion of all coefficients of a polynomial
*              from normal domain to Montgomery domain
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_tomont(poly *r)
{
  unsigned int i;
  const int16_t f = (1ULL << 32) % KYBER_Q;
  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = montgomery_reduce((int32_t)r->coeffs[i]*f);
}

/*************************************************
* Name:        poly_reduce
*
* Description: Applies Barrett reduction to all coefficients of a polynomial
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - poly *r: pointer to input/output polynomial
**************************************************/
void poly_reduce(poly *r)
{
  unsigned int i;
  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = barrett_reduce(r->coeffs[i]);
}

/*************************************************
* Name:        poly_add
*
* Description: Add two polynomials; no modular reduction is performed
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_add(poly *r, const poly *a, const poly *b)
{
  unsigned int i;
  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = a->coeffs[i] + b->coeffs[i];
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract two polynomials; no modular reduction is performed
*
* Arguments: - poly *r:       pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_sub(poly *r, const poly *a, const poly *b)
{
  unsigned int i;
  for(i=0;i<KYBER_N;i++)
    r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
}
/*************** kyber/ref/polyvec.c */

/*************************************************
* Name:        polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
#define KYBER_K 2
void polyvec_compress_2(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES2], const polyvec2 *a)
{
  unsigned int i,j,k;
  uint64_t d0;
  uint16_t t[4];
  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_N/4;j++) {
      for(k=0;k<4;k++) {
        t[k]  = a->vec[i].coeffs[4*j+k];
        t[k] += ((int16_t)t[k] >> 15) & KYBER_Q;
/*      t[k]  = ((((uint32_t)t[k] << 10) + KYBER_Q/2)/ KYBER_Q) & 0x3ff; */
        d0 = t[k];
        d0 <<= 10;
        d0 += 1665;
        d0 *= 1290167;
        d0 >>= 32;
        t[k] = d0 & 0x3ff;
      }

      r[0] = (t[0] >> 0);
      r[1] = (t[0] >> 8) | (t[1] << 2);
      r[2] = (t[1] >> 6) | (t[2] << 4);
      r[3] = (t[2] >> 4) | (t[3] << 6);
      r[4] = (t[3] >> 2);
      r += 5;
    }
  }
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_compress_3(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES3], const polyvec3 *a)
{
  unsigned int i,j,k;
  uint64_t d0;
  uint16_t t[4];
  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_N/4;j++) {
      for(k=0;k<4;k++) {
        t[k]  = a->vec[i].coeffs[4*j+k];
        t[k] += ((int16_t)t[k] >> 15) & KYBER_Q;
/*      t[k]  = ((((uint32_t)t[k] << 10) + KYBER_Q/2)/ KYBER_Q) & 0x3ff; */
        d0 = t[k];
        d0 <<= 10;
        d0 += 1665;
        d0 *= 1290167;
        d0 >>= 32;
        t[k] = d0 & 0x3ff;
      }

      r[0] = (t[0] >> 0);
      r[1] = (t[0] >> 8) | (t[1] << 2);
      r[2] = (t[1] >> 6) | (t[2] << 4);
      r[3] = (t[2] >> 4) | (t[3] << 6);
      r[4] = (t[3] >> 2);
      r += 5;
    }
  }
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_compress_4(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES4], const polyvec4 *a)
{
  unsigned int i,j,k;
  uint64_t d0;
  uint16_t t[8];
  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_N/8;j++) {
      for(k=0;k<8;k++) {
        t[k]  = a->vec[i].coeffs[8*j+k];
        t[k] += ((int16_t)t[k] >> 15) & KYBER_Q;
/*      t[k]  = ((((uint32_t)t[k] << 11) + KYBER_Q/2)/KYBER_Q) & 0x7ff; */
        d0 = t[k];
        d0 <<= 11;
        d0 += 1664;
        d0 *= 645084;
        d0 >>= 31;
        t[k] = d0 & 0x7ff;

      }

      r[ 0] = (t[0] >>  0);
      r[ 1] = (t[0] >>  8) | (t[1] << 3);
      r[ 2] = (t[1] >>  5) | (t[2] << 6);
      r[ 3] = (t[2] >>  2);
      r[ 4] = (t[2] >> 10) | (t[3] << 1);
      r[ 5] = (t[3] >>  7) | (t[4] << 4);
      r[ 6] = (t[4] >>  4) | (t[5] << 7);
      r[ 7] = (t[5] >>  1);
      r[ 8] = (t[5] >>  9) | (t[6] << 2);
      r[ 9] = (t[6] >>  6) | (t[7] << 5);
      r[10] = (t[7] >>  3);
      r += 11;
    }
  }
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
#define KYBER_K 2
void polyvec_decompress_2(polyvec2 *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES2])
{
  unsigned int i,j,k;
  uint16_t t[4];
  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_N/4;j++) {
      t[0] = (a[0] >> 0) | ((uint16_t)a[1] << 8);
      t[1] = (a[1] >> 2) | ((uint16_t)a[2] << 6);
      t[2] = (a[2] >> 4) | ((uint16_t)a[3] << 4);
      t[3] = (a[3] >> 6) | ((uint16_t)a[4] << 2);
      a += 5;

      for(k=0;k<4;k++)
        r->vec[i].coeffs[4*j+k] = ((uint32_t)(t[k] & 0x3FF)*KYBER_Q + 512) >> 10;
    }
  }
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_decompress_3(polyvec3 *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES3])
{
  unsigned int i,j,k;
  uint16_t t[4];
  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_N/4;j++) {
      t[0] = (a[0] >> 0) | ((uint16_t)a[1] << 8);
      t[1] = (a[1] >> 2) | ((uint16_t)a[2] << 6);
      t[2] = (a[2] >> 4) | ((uint16_t)a[3] << 4);
      t[3] = (a[3] >> 6) | ((uint16_t)a[4] << 2);
      a += 5;

      for(k=0;k<4;k++)
        r->vec[i].coeffs[4*j+k] = ((uint32_t)(t[k] & 0x3FF)*KYBER_Q + 512) >> 10;
    }
  }
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_decompress_4(polyvec4 *r, const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES4])
{
  unsigned int i,j,k;
  uint16_t t[8];
  for(i=0;i<KYBER_K;i++) {
    for(j=0;j<KYBER_N/8;j++) {
      t[0] = (a[0] >> 0) | ((uint16_t)a[ 1] << 8);
      t[1] = (a[1] >> 3) | ((uint16_t)a[ 2] << 5);
      t[2] = (a[2] >> 6) | ((uint16_t)a[ 3] << 2) | ((uint16_t)a[4] << 10);
      t[3] = (a[4] >> 1) | ((uint16_t)a[ 5] << 7);
      t[4] = (a[5] >> 4) | ((uint16_t)a[ 6] << 4);
      t[5] = (a[6] >> 7) | ((uint16_t)a[ 7] << 1) | ((uint16_t)a[8] << 9);
      t[6] = (a[8] >> 2) | ((uint16_t)a[ 9] << 6);
      t[7] = (a[9] >> 5) | ((uint16_t)a[10] << 3);
      a += 11;

      for(k=0;k<8;k++)
        r->vec[i].coeffs[8*j+k] = ((uint32_t)(t[k] & 0x7FF)*KYBER_Q + 1024) >> 11;
    }
  }
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
#define KYBER_K 2
void polyvec_tobytes_2(uint8_t r[KYBER_POLYVECBYTES2], const polyvec2 *a)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_tobytes(r+i*KYBER_POLYBYTES, &a->vec[i]);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_tobytes_3(uint8_t r[KYBER_POLYVECBYTES3], const polyvec3 *a)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_tobytes(r+i*KYBER_POLYBYTES, &a->vec[i]);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_tobytes_4(uint8_t r[KYBER_POLYVECBYTES4], const polyvec4 *a)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_tobytes(r+i*KYBER_POLYBYTES, &a->vec[i]);
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of polyvec_tobytes
*
* Arguments:   - uint8_t *r:       pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
*                                  (of length KYBER_POLYVECBYTES)
**************************************************/
#define KYBER_K 2
void polyvec_frombytes_2(polyvec2 *r, const uint8_t a[KYBER_POLYVECBYTES2])
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_frombytes(&r->vec[i], a+i*KYBER_POLYBYTES);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_frombytes_3(polyvec3 *r, const uint8_t a[KYBER_POLYVECBYTES3])
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_frombytes(&r->vec[i], a+i*KYBER_POLYBYTES);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_frombytes_4(polyvec4 *r, const uint8_t a[KYBER_POLYVECBYTES4])
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_frombytes(&r->vec[i], a+i*KYBER_POLYBYTES);
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_ntt
*
* Description: Apply forward NTT to all elements of a vector of polynomials
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
#define KYBER_K 2
void polyvec_ntt_2(polyvec2 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_ntt(&r->vec[i]);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_ntt_3(polyvec3 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_ntt(&r->vec[i]);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_ntt_4(polyvec4 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_ntt(&r->vec[i]);
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_invntt_tomont
*
* Description: Apply inverse NTT to all elements of a vector of polynomials
*              and multiply by Montgomery factor 2^16
*
* Arguments:   - polyvec *r: pointer to in/output vector of polynomials
**************************************************/
#define KYBER_K 2
void polyvec_invntt_tomont_2(polyvec2 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_invntt_tomont(&r->vec[i]);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_invntt_tomont_3(polyvec3 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_invntt_tomont(&r->vec[i]);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_invntt_tomont_4(polyvec4 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_invntt_tomont(&r->vec[i]);
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_basemul_acc_montgomery
*
* Description: Multiply elements of a and b in NTT domain, accumulate into r,
*              and multiply by 2^-16.
*
* Arguments: - poly *r: pointer to output polynomial
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
#define KYBER_K 2
void polyvec_basemul_acc_montgomery_2(poly *r, const polyvec2 *a, const polyvec2 *b)
{
  unsigned int i;
  poly t;

  poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);
  for(i=1;i<KYBER_K;i++) {
    poly_basemul_montgomery(&t, &a->vec[i], &b->vec[i]);
    poly_add(r, r, &t);
  }

  poly_reduce(r);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_basemul_acc_montgomery_3(poly *r, const polyvec3 *a, const polyvec3 *b)
{
  unsigned int i;
  poly t;

  poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);
  for(i=1;i<KYBER_K;i++) {
    poly_basemul_montgomery(&t, &a->vec[i], &b->vec[i]);
    poly_add(r, r, &t);
  }

  poly_reduce(r);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_basemul_acc_montgomery_4(poly *r, const polyvec4 *a, const polyvec4 *b)
{
  unsigned int i;
  poly t;

  poly_basemul_montgomery(r, &a->vec[0], &b->vec[0]);
  for(i=1;i<KYBER_K;i++) {
    poly_basemul_montgomery(&t, &a->vec[i], &b->vec[i]);
    poly_add(r, r, &t);
  }

  poly_reduce(r);
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_reduce
*
* Description: Applies Barrett reduction to each coefficient
*              of each element of a vector of polynomials;
*              for details of the Barrett reduction see comments in reduce.c
*
* Arguments:   - polyvec *r: pointer to input/output polynomial
**************************************************/
#define KYBER_K 2
void polyvec_reduce_2(polyvec2 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_reduce(&r->vec[i]);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_reduce_3(polyvec3 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_reduce(&r->vec[i]);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_reduce_4(polyvec4 *r)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_reduce(&r->vec[i]);
}
#undef KYBER_K

/*************************************************
* Name:        polyvec_add
*
* Description: Add vectors of polynomials
*
* Arguments: - polyvec *r: pointer to output vector of polynomials
*            - const polyvec *a: pointer to first input vector of polynomials
*            - const polyvec *b: pointer to second input vector of polynomials
**************************************************/
#define KYBER_K 2
void polyvec_add_2(polyvec2 *r, const polyvec2 *a, const polyvec2 *b)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
}
#undef KYBER_K

#define KYBER_K 3
void polyvec_add_3(polyvec3 *r, const polyvec3 *a, const polyvec3 *b)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
}
#undef KYBER_K

#define KYBER_K 4
void polyvec_add_4(polyvec4 *r, const polyvec4 *a, const polyvec4 *b)
{
  unsigned int i;
  for(i=0;i<KYBER_K;i++)
    poly_add(&r->vec[i], &a->vec[i], &b->vec[i]);
}
#undef KYBER_K

/*************** kyber/ref/reduce.c */

/*************************************************
* Name:        montgomery_reduce
*
* Description: Montgomery reduction; given a 32-bit integer a, computes
*              16-bit integer congruent to a * R^-1 mod q, where R=2^16
*
* Arguments:   - int32_t a: input integer to be reduced;
*                           has to be in {-q2^15,...,q2^15-1}
*
* Returns:     integer in {-q+1,...,q-1} congruent to a * R^-1 modulo q.
**************************************************/
int16_t montgomery_reduce(int32_t a)
{
  int16_t t;

  t = (int16_t)a*QINV;
  t = (a - (int32_t)t*KYBER_Q) >> 16;
  return t;
}

/*************************************************
* Name:        barrett_reduce
*
* Description: Barrett reduction; given a 16-bit integer a, computes
*              centered representative congruent to a mod q in {-(q-1)/2,...,(q-1)/2}
*
* Arguments:   - int16_t a: input integer to be reduced
*
* Returns:     integer in {-(q-1)/2,...,(q-1)/2} congruent to a modulo q.
**************************************************/
int16_t barrett_reduce(int16_t a) {
  int16_t t;
  const int16_t v = ((1<<26) + KYBER_Q/2)/KYBER_Q;

  t  = ((int32_t)v*a + (1<<25)) >> 26;
  t *= KYBER_Q;
  return a - t;
}
/*************** kyber/ref/symmetric-shake.c */

/*************************************************
* Name:        kyber_shake128_absorb
*
* Description: Absorb step of the SHAKE128 specialized for the Kyber context.
*
* Arguments:   - keccak_state *state: pointer to (uninitialized) output Keccak state
*              - const uint8_t *seed: pointer to KYBER_SYMBYTES input to be absorbed into state
*              - uint8_t i: additional byte of input
*              - uint8_t j: additional byte of input
**************************************************/
void kyber_shake128_absorb(keccak_state *state,
                           const uint8_t seed[KYBER_SYMBYTES],
                           uint8_t x,
                           uint8_t y)
{
  uint8_t extseed[KYBER_SYMBYTES+2];

  memcpy(extseed, seed, KYBER_SYMBYTES);
  extseed[KYBER_SYMBYTES+0] = x;
  extseed[KYBER_SYMBYTES+1] = y;

  shake128_absorb_once(state, extseed, sizeof(extseed));
}

/*************************************************
* Name:        kyber_shake256_prf
*
* Description: Usage of SHAKE256 as a PRF, concatenates secret and public input
*              and then generates outlen bytes of SHAKE256 output
*
* Arguments:   - uint8_t *out: pointer to output
*              - size_t outlen: number of requested output bytes
*              - const uint8_t *key: pointer to the key (of length KYBER_SYMBYTES)
*              - uint8_t nonce: single-byte nonce (public PRF input)
**************************************************/
void kyber_shake256_prf(uint8_t *out, size_t outlen, const uint8_t key[KYBER_SYMBYTES], uint8_t nonce)
{
  uint8_t extkey[KYBER_SYMBYTES+1];

  memcpy(extkey, key, KYBER_SYMBYTES);
  extkey[KYBER_SYMBYTES] = nonce;

  shake256(out, outlen, extkey, sizeof(extkey));
}

/*************************************************
* Name:        kyber_shake256_prf
*
* Description: Usage of SHAKE256 as a PRF, concatenates secret and public input
*              and then generates outlen bytes of SHAKE256 output
*
* Arguments:   - uint8_t *out: pointer to output
*              - size_t outlen: number of requested output bytes
*              - const uint8_t *key: pointer to the key (of length KYBER_SYMBYTES)
*              - uint8_t nonce: single-byte nonce (public PRF input)
**************************************************/
void kyber_shake256_rkprf2(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES2])
{
  keccak_state s;

  shake256_init(&s);
  shake256_absorb(&s, key, KYBER_SYMBYTES);
  shake256_absorb(&s, input, KYBER_CIPHERTEXTBYTES2);
  shake256_finalize(&s);
  shake256_squeeze(out, KYBER_SSBYTES, &s);
}

void kyber_shake256_rkprf3(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES3])
{
  keccak_state s;

  shake256_init(&s);
  shake256_absorb(&s, key, KYBER_SYMBYTES);
  shake256_absorb(&s, input, KYBER_CIPHERTEXTBYTES3);
  shake256_finalize(&s);
  shake256_squeeze(out, KYBER_SSBYTES, &s);
}

void kyber_shake256_rkprf4(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES4])
{
  keccak_state s;

  shake256_init(&s);
  shake256_absorb(&s, key, KYBER_SYMBYTES);
  shake256_absorb(&s, input, KYBER_CIPHERTEXTBYTES4);
  shake256_finalize(&s);
  shake256_squeeze(out, KYBER_SSBYTES, &s);
}
