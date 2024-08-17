//
// Created by Adrien COURNAND on 17/08/2024.
//

#include <cstdio>
#include <cstdlib>

#include "vec.hpp"

int main(void)
{
	UVec4 p1,p2;

	p2.pos.x += 1;

	p1.val += p2.val;

	printf("%.2f   %.2f   %.2f   %.2f\t",p1.pos.x,p1.pos.y,p1.pos.z,p1.pos.t);
	return EXIT_SUCCESS;
}