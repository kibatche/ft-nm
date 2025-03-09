int main() {
   
 
    // Getting strings input
   char *str1 = "vendor/golang.org/x/crypto/cryptobyte.(*String).ReadASN1";
   char *str2 = "vendor/golang.org/x/crypto/cryptobyte.(*String).SeadASN1";

   printf("strcmp : %d\n", strcmp(str1, str2));
    return 0;
 }