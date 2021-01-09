#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length)
{
  char append[16];
  strcpy(output, "");
  for (int n = 0; n < hash_length; n++)
  {
    sprintf(append, "%02x", str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest)
{
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1((const unsigned char *)text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

int leading_zeros(const char *digest)
{
  for (int i = 0; i < (int)strlen(digest); i++)
  {
    if (!((digest[i] >= '0' && digest[i] <= '9') || (digest[i] >= 'a' && digest[i] <='f')))
    {
      return -1;
    }
  }
  int num_zero = 0;
  for (int i = 0; digest[i] == '0'; i++)
  {
    num_zero++;
  }
  return num_zero;
}

bool file_to_SHA1_digest(const char *filename, char *digest)
{
  ifstream input;
  char temp;
  string text;

  input.open(filename);
  if (input.fail())
  {
    digest = "error";
    return false;
  }
  else
  {
    input.get(temp);
    while (!input.eof())
    {
      text+=temp;
      input.get(temp);
    }
  }
  text_to_SHA1_digest(text.c_str(), digest);
  return true;
}

void convert_int_to_char(int integ, char * charr){

  int num_reverse[11];
  int i=0;
  int z=0;
  while(integ!=0){
    int temp = integ%10;
    num_reverse[i++]=temp;
    integ=integ/10;
  }
  for(int j=i-1;j>-1;j--){
    charr[z++]=(char)(num_reverse[j]+48);
  }
  charr[z]='\0';
}

bool make_header(const char *recipient, const char *filename, char *header)
{
  char digest[41];
  int counter_pos;
  if (!file_to_SHA1_digest(filename, digest))
  {
    return false;
  }

  //initialize the header
  else
  {
    //add the header
    int i;
    for (i = 0; recipient[i] != '\0'; i++)
    {
      header[i] = recipient[i];
    }
    header[i++] = ':';

    //add the digest
    for (int y = 0; digest[y] != '\0'; y++)
    {
      header[i++] = digest[y];
    }
    header[i++] = ':';
    //add the counter
     counter_pos = i;
     cout<<counter_pos<<"***";
     cout<<header;
    // header[i++] = '0';
   // header[i] = '\0';
  }

  //get the digest of the header
  char header_digest[41];
  char counter[10];
  for (int z = 0; z < 10000000; z++)
  {
      int temp_counter_pos=counter_pos;
      convert_int_to_char(z,counter);
    for(int j=0;counter[j]!='\0';j++){
      header[temp_counter_pos++]=counter[j];
    }
    header[temp_counter_pos]='\0';
    text_to_SHA1_digest(header, header_digest);

    if (leading_zeros(header_digest) > 4)
    {
     // cout<<header<<endl;
      return true;
    }
    

  }
  return false;
}

/* add your function definitions here */