#ifndef TOKENS_HH
#define TOKENS_HH

#include "bool_decoder.hh"

enum token {
  ZERO_TOKEN,
  ONE_TOKEN,
  TWO_TOKEN,
  THREE_TOKEN,
  FOUR_TOKEN,
  DCT_VAL_CATEGORY1,
  DCT_VAL_CATEGORY2,
  DCT_VAL_CATEGORY3,
  DCT_VAL_CATEGORY4,
  DCT_VAL_CATEGORY5,
  DCT_VAL_CATEGORY6,
  DCT_EOB_TOKEN
};

const unsigned int ENTROPY_NODES = DCT_VAL_CATEGORY6 + 1;
const unsigned int MAX_ENTROPY_TOKENS = DCT_EOB_TOKEN + 1;

const extern TreeArray< MAX_ENTROPY_TOKENS > token_tree;

static const SafeArray< uint8_t, 16 > coefficient_to_band {{ 0, 1, 2, 3, 6, 4, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7 }};
static const SafeArray< uint8_t, 16 > zigzag = {{ 0, 1, 4, 8, 5, 2, 3, 6, 9, 12, 13, 10, 7, 11, 14, 15 }};

template < unsigned int length >
struct TokenDecoder
{
private:
  const uint16_t base_value_;
  const SafeArray< Probability, length > bit_probabilities_;

public:
  TokenDecoder( const uint16_t base_value, const SafeArray< Probability, length > & probs )
    : base_value_( base_value ), bit_probabilities_( probs ) {}

  uint16_t decode( BoolDecoder & data ) const
  {
    uint16_t increment = 0;
    for ( uint8_t i = 0; i < length; i++ ) { increment = ( increment << 1 ) + data.get( bit_probabilities_.at( i ) ); }
    return base_value_ + increment;
  }
};

static const TokenDecoder< 2 > token_decoder_1( 7, { 165, 145 } );
static const TokenDecoder< 3 > token_decoder_2( 11, { 173, 148, 140 } );
static const TokenDecoder< 4 > token_decoder_3( 19, { 176, 155, 140, 135 } );
static const TokenDecoder< 5 > token_decoder_4( 35, { 180, 157, 141, 134, 130 } );
static const TokenDecoder< 11 > token_decoder_5( 67, { 254, 254, 243, 230, 196, 177, 153, 140, 133, 130, 129 } );

#endif /* TOKENS_HH */
