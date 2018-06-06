#include "core.h"
#include "groups.h"

//=======================================================
// main
//=======================================================
int main( int argc, char **argv )
{
	int group[2] = {0, 0};

	if(argc == 3) {
		group[0] = atoi( argv[1] );
		group[1] = atoi( argv[2] );
	}

	while(true) {

		printf("============================================================\n");

		if(!group[0]){
			printf("先攻のグループ番号："); scanf("%d", &group[0]);
			if(!group[0]) break;
		}

		if(!group[1]){
			printf("後攻のグループ番号："); scanf("%d", &group[1]);
			if(!group[1]) break;
		}

		battle(group);

		group[0] = 0;
		group[1] = 0;
	}

	system("pause");
    return 0;
}
