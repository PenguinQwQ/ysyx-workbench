#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s != NULL);
  size_t len = 0;
  while(*s != '\0') 
    {
      len++; s++;
    }
  return len;
}

char *strcpy(char *dst, const char *src) {
  return strncpy(dst, src, strlen(src));
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i = 0;
  for (i = 0 ; i < n && src[i] != '\0'; i++) 
    dst[i] = src[i];
  for (; i < n ; i++)
    dst[i] = '\0';
  return dst;
}

char *strncat(char *dst, const char *src, size_t n)
{
  size_t i, dst_len = strlen(dst);
  for (i = 0 ; i < n && src[i] != '\0' ; i++)
    dst[dst_len + i] = src[i];
  dst[dst_len + i] = '\0';
  return dst;
}


char *strcat(char *dst, const char *src) {
  return strncat(dst, src, strlen(src));
}

int strcmp(const char *p1, const char *p2) {
  while((*p1 != '\0') && (*p2 != '\0'))
    {
      if(*p1 < *p2) return -1;
      if(*p1 > *p2) return 1;
      p1++; p2++;
    }
    if(*p1 < *p2) return -1;
    if(*p1 > *p2) return 1;
    return 0;
}

int strncmp(const char *p1, const char *p2, size_t n) {
  size_t i = 0;
  while((*p1 != '\0') && (*p2 != '\0') && (i < n))
    {
      if(*p1 < *p2) return -1;
      if(*p1 > *p2) return 1;
      i++;
      p1++; p2++;
    }
  if(i < n)
  {
    if(*p1 < *p2) return -1;
    if(*p1 > *p2) return 1;
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  unsigned char ch = (unsigned char)c;
  unsigned char *m = (unsigned char *)s;
  for (int i = 0 ; i < n ; i++)
    m[i] = ch;
  return (void *)m;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  unsigned char *p1 = (unsigned char *)s1;
  unsigned char *p2 = (unsigned char *)s2;
  for (int i = 0 ; i < n ; i++)
    {
      if(p1[i] < p2[i]) return -1;
      if(p1[i] > p2[i]) return 1;
    }
  return 0;
}

#endif
