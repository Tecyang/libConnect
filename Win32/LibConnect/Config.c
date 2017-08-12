#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
#define MAX_BUF_LEN 1024  
#define MAX_KEY_LEN 64  
#define MAX_VAL_LEN 256  
  
int Trim(char s[])  
{  
    int n;  
    for(n = strlen(s) - 1; n >= 0; n--)  
    {  
        if(s[n]!=' ' && s[n]!='\t' && s[n]!='\n')  
            break;  
        s[n+1] = '\0';  
    }  
    return n;  
}  
  
int loadConfigDemo(const char* config_path)  
{  
    FILE * file = fopen(config_path, "r");  
    if (file == NULL)  
    {  
        printf("[Error]open %s failed.\n", config_path);  
        return -1;  
    }  
  
  
    char buf[MAX_BUF_LEN];  
    int text_comment = 0;  
    while(fgets(buf, MAX_BUF_LEN, file) != NULL)  
    {  
        Trim(buf);  
        // to skip text comment with flags /* ... */  
        if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/'))  
        {  
            if (strstr(buf, "/*") != NULL)  
            {  
                text_comment = 1;  
                continue;  
            }  
            else if (strstr(buf, "*/") != NULL)  
            {  
                text_comment = 0;  
                continue;  
            }  
        }  
        if (text_comment == 1)  
        {  
            continue;  
        }  
  
  
        int buf_len = strlen(buf);  
        // ignore and skip the line with first chracter '#', '=' or '/'  
        if (buf_len <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/')  
        {  
            continue;  
        }  
        buf[buf_len-1] = '\0';  
  
  
        char _paramk[MAX_KEY_LEN] = {0}, _paramv[MAX_VAL_LEN] = {0};  
        int _kv=0, _klen=0, _vlen=0;  
        int i = 0;  
        for (i=0; i<buf_len; ++i)  
        {  
            if (buf[i] == ' ')  
                continue;  
            // scan param key name  
            if (_kv == 0 && buf[i] != '=')  
            {  
                if (_klen >= MAX_KEY_LEN)  
                    break;  
                _paramk[_klen++] = buf[i];  
                continue;  
            }  
            else if (buf[i] == '=')  
            {  
                _kv = 1;  
                continue;  
            }  
            // scan param key value  
            if (_vlen >= MAX_VAL_LEN || buf[i] == '#')  
                break;  
            _paramv[_vlen++] = buf[i];  
        }  
        if (strcmp(_paramk, "")==0 || strcmp(_paramv, "")==0)  
            continue;  
        printf("%s=%s\n", _paramk, _paramv);  
    }  
    return 0;  
}  
  
