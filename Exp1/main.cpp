#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/**
 *  ******ʵ��һ  �ʷ�����******
 *  A simple Lexical Analyzer for PL/0 language
 *  written by xizijun, @ 2020/5/13
 *  all rights reserved.
 */
const char *FILENAME = "demo.txt"; ///ʾ��PL/0Դ�����ļ�·��

struct Word {
    int num; ///����������
    char w[10]; ///�ʵ�����ֵ
};

char KEYWORDS[28][10]= {
    "begin","end","if","then","while","do","const","var","call","procedure",
    "+","-","*","/","odd","=", "<>", "<",">","<=",">=",":=","(",")",",",".",";","#"
};

bool IsIDLegal(char* c) { ///�жϱ�ʶ���Ƿ�Ϸ�
    if(strlen(c)>10)
        return false;
    for(int i=0; i<strlen(c); i++) {
        if(!isalpha(c[i]))
            return false;
    }
    return true;
}

bool IsNumLegal(char* num) { ///�жϳ����Ƿ�Ϸ�
    for(int i=0; i<strlen(num); i++) {
        if(!isdigit(num[i]))
            return false;
    }
    return true;
}

void PL0SampleCodeShow() { ///��ȡ����ʾPL0ʾ�������ļ�
    FILE *fp;
    char ch;
    fp=fopen(FILENAME, "r");
    if(!fp) {
        printf("�޷����ļ�: %s !\n", FILENAME);
        exit(1);
    }
    printf("ʾ��PL/0��������:\n");
    while((ch=fgetc(fp))!=EOF) {
        putchar(ch);
    }
    fclose(fp);
}

void WordsClassTable() { ///������������˵��
    puts("\n\n############################");
    printf("��������:\n");
    printf("0 : �޷�ʶ�� (ERROR) \n");
    printf("1 : �ؼ��� (10��) \n");
    printf("2 : ��ʶ�� (��ĸ���У���󳤶�10)\n");
    printf("3 : ���� (��������)\n");
    printf("4-21 : ���/��� (18��) \n");
    puts("############################");
    printf("\n�ʷ�������, ��������ĵ��ʷ�����:\n");
}

void LexicalAnalysis() { ///�ʷ�����
    char ch, word_temp[20]="";  ///������
    int i=0, j=0, k=0, key=0, flag=0;
    FILE *fp; ///�����ļ�
    Word word[100]; ///�����ʶ���
    PL0SampleCodeShow(); ///PL0ʾ������չʾ
    fp=fopen(FILENAME, "r"); ///���ļ�
    while((ch=fgetc(fp)) != EOF) { ///�����ӳ���ÿ��ֻ��ȡһ�����ʣ�ֱ���ļ�ĩβ
        if(isalnum(ch)) { ///�ж���ĸ������
            word_temp[key++] = ch;  ///�����ĸ����������
            word_temp[key] = '\0';
            continue;
        } else {
            if(strcmp(word_temp, "")!=0) {
                strcpy(word[i].w, word_temp); ///����ĸ���������п������ṹ������
                strcpy(word_temp, ""); ///��ʱ�����ÿ�
                key=0; ///�ص���ʱ����Ŀ�ʼλ��
                i++; ///�ṹ������±��1
            }
            if(ch==' '|| ch==10 || ch==13 || ch=='	') ///ȥ���ո񡢻س������к��Ʊ��
                continue;
            else { ///�ж����������ϡ����/���
                word_temp[0] = ch, flag = 0;
                if(word_temp[0]==':' || word_temp[0]=='>'|| word_temp[0]=='<')
                    ch=fgetc(fp), flag = 1;
                if((ch=='=' && flag) || (word_temp[0]=='<' && ch=='>'))
                    word_temp[1]=ch, word_temp[2]='\0';
                else
                    word_temp[1]='\0';
                strcpy(word[i].w, word_temp); ///������ĸ�����ַ������п������ṹ������
                strcpy(word_temp, "");
                key=0, i++;
            }
        }
    }
    ///�����
    for(j=0; j<i; j++) {
        for(k=0; k<28; k++) {
            if((strcmp(word[j].w, KEYWORDS[k]))==0) {
                if(k>=0 && k<10)
                    word[j].num=1; ///�ؼ���
                else if(k>=10 && k<28)
                    word[j].num=k; ///���/���
                break;
            } else if(IsIDLegal(word[j].w))
                word[j].num=2; ///��ʶ��
            else if(IsNumLegal(word[j].w))
                word[j].num=3; ///����
            else
                word[j].num=0; ///�޷�ʶ��
        }
    }
    WordsClassTable(); ///��ӡ���������
    for(j=0; j<i; j++) { ///��ʽ����ӡ���, ��(����, ����ֵ)
        if(word[j].num == 1)
            printf("(%d,)\t\t%s%s\n", word[j].num, "�ؼ���", word[j].w);
        else if(word[j].num == 2)
            printf("(%d,\'%s\')\t\t%s%s\n", word[j].num, word[j].w, "��ʶ��", word[j].w);
        else if(word[j].num == 3)
            printf("(%d,\'%s\')\t\t%s%s\n", word[j].num, word[j].w, "����", word[j].w);
        else if(word[j].num == 0)
            printf("(%d,)\t\t%s%s\n", word[j].num, "ERROR: ", word[j].w);
        else {
            if(!(strcmp("+", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "�Ӻ�", word[j].w);
            else if(!(strcmp("-", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "����", word[j].w);
            else if(!(strcmp("*", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "�˺�", word[j].w);
            else if(!(strcmp("/", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "����", word[j].w);
            else if(!(strcmp("odd", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "������", word[j].w);
            else if(!(strcmp("=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "���ں�", word[j].w);
            else if(!(strcmp("<>", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "�����ں�", word[j].w);
            else if(!(strcmp("<", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "С�ں�", word[j].w);
            else if(!(strcmp(">", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "���ں�", word[j].w);
            else if(!(strcmp("<=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "С�ڵ��ں�", word[j].w);
            else if(!(strcmp(">=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "���ڵ��ں�", word[j].w);
            else if(!(strcmp(":=", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "��ֵ��", word[j].w);
            else if(!(strcmp("(", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "������", word[j].w);
            else if(!(strcmp(")", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "������", word[j].w);
            else if(!(strcmp(",", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "����", word[j].w);
            else if(!(strcmp(".", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "�ս��", word[j].w);
            else if(!(strcmp(";", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "��ĩ��", word[j].w);
            else if(!(strcmp("#", KEYWORDS[word[j].num])))
                printf("(%d,)\t\t%s%s\n", word[j].num-6, "��ת��", word[j].w);
            else
                printf("(%d,)\t\t%s%s\n", 0, "ERROR", word[j].w);
        }

    }
    fclose(fp); ///�ر��ļ�
}

int main() {
    LexicalAnalysis();
    return 0;
}
