#include <stdio.h>

int main()
{
	int blockIdx, threadIdx, blockDim=77, len=200, slide=50, 
		total_windows=45825;

	for (blockIdx = 0; blockIdx < 10; blockIdx++) {
		for (threadIdx = 0; threadIdx < blockDim; threadIdx++) {
			int i = blockIdx * blockDim + threadIdx;
			//if (threadIdx >= (blockDim - len/slide)) {
					//}
			int j;
			printf("%d:", threadIdx);
			for (j = threadIdx*slide; j < (threadIdx*slide + slide); j++) {
				printf(" %d",j);
			}
			printf("\n");
		}
	}
	
}
