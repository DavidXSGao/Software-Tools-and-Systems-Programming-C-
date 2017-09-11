#include <stdio.h>
int main(){
	int ind, amt;
	int num_array[10]={0};
	while(scanf("%d %d", &ind, &amt) != EOF){
	num_array[ind] += amt;
	}
	int i;
	for(i=0; i<10; i++){
	printf("array[%d] = %d, ", i,num_array[i]);
	}
	return 0; 
}