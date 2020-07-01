#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/**
 *  ******ʵ���  �﷨����******
 *  A simple Syntax Analyzer for Arithmetic expression
 *  written by xizijun, @ 2020/5/30
 *  all rights reserved.
 *
 */
const char *FILENAME = "expr.txt"; /// �������ʽ����

#define ADVANCE() (idx<CNT-1? ++idx : threshold=1)
int threshold = 0;
int ERROR = 0, CNT = 0;
int SYM[100];
int idx = 0;

/** ------------------------------------�ʷ�������--------------------------------------*/
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
    printf("�������ʽ����:\n");
    while((ch=fgetc(fp))!=EOF) {
        putchar(ch);
    }
    fclose(fp);
}

void WordsClassTable() { ///������������˵��
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
    CNT = i;
    for(j=0; j<i; j++) { ///��ʽ����ӡ���, ��(����, ����ֵ)
        SYM[j] = word[j].num>=10? word[j].num-6:word[j].num; //������������SYM����
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
/** ----------------------------------�﷨������--------------------------------*/
void E();
void E1();
void T();
void T1();
void F();

void E() { //E��TE'
    T(); E1();
}
void E1() { //E'��+TE'|-TE'|��
    if(SYM[idx] == 4) { // ��ǰSYMΪ"+"
        ADVANCE();
        T(); E1();
    } else if(SYM[idx] == 5) { // ��ǰSYMΪ"-"
        ADVANCE();
        T(); E1();
    } else if(SYM[idx]!=17 && threshold!=1) { //SYM��Ϊ")"��idx�������ʽ����
        ERROR = -1; return;
    }
}
void T() { //T��FT'
    F(); T1();
}
void T1() { //T'��*FT'|/FT'|��
    if(SYM[idx] == 6) { // ��ǰSYMΪ"*"
        ADVANCE();
        F(); T1();
    } else if(SYM[idx] == 7) { // ��ǰSYMΪ"/"
        ADVANCE();
        F(); T1();
    } else if(SYM[idx]!=4 && SYM[idx]!=5 && SYM[idx]!=17 && threshold!=1) {
        //SYM��Ϊ")"��"+"��"-"��idx�������ʽ����
        ERROR = -1; return;
    }
}
void F() { //F��(E)|id|num
    if(SYM[idx]==2 || SYM[idx]==3) { // ��ǰSYMΪ��ʶ��, ����
        ADVANCE();
    } else {
        if(SYM[idx] == 16) { // ��ǰSYMΪ"("
            ADVANCE();
            E();
            if(SYM[idx] == 17)  // ��ǰSYMΪ")"
                ADVANCE();
            else {
                ERROR = -1; return;
            }
        } else {
            ERROR = -1; return;
        }
    }
}
void SyntaxAnalysis() {
    puts("\n�﷨����������£�");
    E();
    if(ERROR == -1 || !threshold)
        printf("Wrong: �ñ��ʽ�﷨����\n");
    else
        printf("Correct: �ñ��ʽ�﷨��ȷ��\n");
}
int main() {
    LexicalAnalysis();
    SyntaxAnalysis();
    return 0;
}
