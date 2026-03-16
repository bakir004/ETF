#include <stdio.h>
void debug ( )
{
  printf("SECRET!\n");
}
int checkPassword (){
  char password[64];
  printf("Password: ");
  gets (password) ;
  return 0;
}
int main(int argc, char **argv){
  printf( "WELCOME TO THE SECURE SERVER\n");
  if (checkPassword()){
    debug();
  } else {
    printf("Nista\n");
  }
}
