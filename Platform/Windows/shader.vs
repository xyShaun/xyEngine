#include "a2v.h"
#include "v2p.h"

v2p main(a2v i)
{
	v2p o;
	
	o.position = float4(i.position, 1.0);
	o.color = i.color;
	
	return o;
}