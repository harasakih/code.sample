
_bitkey = [ 
    'LIGHT_R', 'LIGHT_Y', 'LIGHT_G',
    'BUZZ_S', 'BUZZ_L',
    'BLINK_R', 'BLINK_Y', 'BLINK_G' ]

_bit = {
	'LIGHT_R' : 0,
	'LIGHT_Y' : 1,
	'LIGHT_G' : 2,
	'BUZZ_S'  : 3,
	'BUZZ_L'  : 4,
	'BLINK_R' : 5,
	'BLINK_Y' : 6,
	'BLINK_G' : 7
}



bitstr = "01234567"
for i in range(8):
    bit = bitstr[i:i+1]
    print( i, bit )
    print( _bitkey[i], _bit[ _bitkey[i] ] )
