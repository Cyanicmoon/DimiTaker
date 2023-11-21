#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "ImageLayer.h"


#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define ENTER 4
#define RESET 5


#pragma comment(lib,"winmm.lib")


INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;


//DWORD CIN;
DWORD mode;


struct player {
    char name[100];
    int stage;

} player;

enum color {
    Black = 0,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    Lgray,
    Dgray,
    Lblue,
    Lgreen,
    Lcyan,
    Lred,
    Lmagenta,
    Yellow,
    White
};


//mci�� ����Ͽ� �Ҹ��� ����ϱ� ���� �Լ�
void playSound(TCHAR* fileName) {
    TCHAR command[100];
    wsprintf(command, TEXT("play %s"), fileName);
    mciSendString(command, NULL, 0, NULL);
}

//������̴� �Ҹ��� ���߱� ���� �Լ�
void stopSound(wchar_t* fileName) {

    wchar_t command[100];
    wsprintf(command, TEXT("stop %s"), fileName);
    mciSendString(command, NULL, 0, NULL);
}



int getKey() { // Ű���� �Է¿� ���� �ش� �� ��ȯ
    char t = getch();

    if (t == 'w' || t == 'W') {
        return UP;
    }
    else if (t == 'S' || t == 's') {
        return DOWN;
    }
    else if (t == 'a' || t == 'A') {
        return LEFT;
    }
    else if (t == 'd' || t == 'D') {
        return RIGHT;
    }
    else if (t == ' ') {
        return ENTER;
    }
    else if (t == 'r' || t == 'R') {
        return RESET;
    }
}

void color(int text, int bg) { // ����, ���� ����
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + bg * 16);
}

void gotoxy(int x, int y) { // Ŀ�� ��ġ �̵� �Լ�
    COORD Pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void renderImg(char dir[], int x, int y, double scale) {
    ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image images[1] = {
        {dir, x, y, scale} //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 1; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = images;
    
    //imageLayer.fadeIn(&imageLayer, 1);

    imageLayer.renderAll(&imageLayer);
}



int menu() { // ���� ���۽� �޴� ���
    //playSound(L"lobby.wav");
    PlaySound(L"lobby.wav", NULL, SND_ASYNC | SND_LOOP);

    color(Red, Black);
    printf("\n\n\n\n\n   _____   _______  _______  _______     _______  _______  __  __  _______  ______\n");
    printf("  |     \\ |_     _||   |   ||_     _|   |_     _||   _   ||  |/  ||    ___||   __ \\\n");
    printf("  |  --  | _|   |_ |       | _|   |_      |   |  |       ||     < |    ___||      <\n");
    printf("  |_____/ |_______||__|_|__||_______|     |___|  |___|___||__|\\__||_______||___|__|\n\n\n\n");
    color(Lgray, Black);

    int x = 37, y = 13;
    gotoxy(x - 3, y);
    printf("��  ���� �����\n");
    gotoxy(x, y + 1);
    printf("���� �ҷ�����\n");
    gotoxy(x, y + 2);
    printf("    ����");

    while (1) {
        int k = getKey();
        if (k == UP) {
            if (y > 13) {
                stopSound(L"select_move.wav");
                playSound(L"select_move.wav");
                gotoxy(x - 3, y);
                printf("  ");
                gotoxy(x - 3, --y);
                printf("�� "); // ���� Ű���� �Է¿� ���� �̵�
            }
        }
        if (k == DOWN) {
            if (y < 15) {
                stopSound(L"select_move.wav");
                playSound(L"select_move.wav");
                gotoxy(x - 3, y);
                printf("  ");
                gotoxy(x - 3, ++y);
                printf("�� "); // ���� ����
            }
        }

        if (k == ENTER) {
            playSound(L"select_button.wav");
            return y - 13;
        }
    }
    return;
}

void cursurSetting(int a) { // Ŀ�� ���� �Լ� (������ ����)
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = a;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void init() { // �ʱ� ���� �Լ�
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
    system("mode con cols=86 lines=25 | title ��� ����Ŀ");
    cursurSetting(0);
}

void typingEffect(int d, char s[]) { // ���� Ÿ���� ȿ�� �Լ�
    for (int i = 0; s[i] != NULL; i++) {
        printf("%c", s[i]);
        Sleep(d);
    }
}

void loadData() { // ���� ���൵ �ҷ�����
    FILE* fp = fopen("data.txt", "r");
    char temp[100];
    fscanf(fp, "%s\n%d", temp, &player.stage);
    fclose(fp);
    strcpy(player.name, temp);
}

void saveData() { // ���� ���൵ ����
    FILE* fp = fopen("data.txt", "w");
    fprintf(fp, "%s\n%d", player.name, player.stage);
    fclose(fp);
}

void makeNewGame() { // ���� ���� ���� �ܰ�
    gotoxy(36, 10);
    typingEffect(10, "����� �̸���?\n");
    gotoxy(36, 11);
    printf("�� ");
    scanf("%s", player.name);
    player.stage = 0;
    system("cls");

    return;
}

void prologue() { // ���ѷα� ���� ����
    system("cls");
    gotoxy(22, 11);
    typingEffect(10, "����� �ұ��� ���� �ٴ� �ῡ�� ������ϴ�");
    getch();

    system("cls");
    gotoxy(15, 11);
    typingEffect(13, "�� ���� �ٷ� ���ɳ��� ���ǰ�縦 �����Ѵٴ� �����̾���");
    getch();

    system("cls");
    gotoxy(17, 11);
    typingEffect(13, "������ ����� �� ���� ���Ƿ� ��Ÿ�� ���� �����߰�,");
    getch();

    system("cls");
    gotoxy(22, 11);
    typingEffect(13, "�̸� �������� �⳪�� ������ �����߽��ϴ�");
    getch();

}

void showScreen() { // ȭ�� ��ȯ �ƾ� ����
    stopSound("lobby.wav");
    stopSound(L"screen_changer_part1_01.wav");
    stopSound(L"screen_changer_part2_01.wav");
    playSound(L"screen_changer_part1_01.wav");
    Sleep(50);
    renderImg(L"transition0002.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0003.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0004.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0005.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0006.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0007.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0008.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0009.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0010.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0011.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0012.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0013.bmp", 0, 0, 0.72);
    Sleep(500);
    playSound(L"screen_changer_part2_01.wav");
    Sleep(50);
    renderImg(L"transition0023.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0024.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0025.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0026.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0027.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0028.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0029.bmp", 0, 0, 0.72);
    Sleep(50);
    renderImg(L"transition0030.bmp", 0, 0, 0.72);
    
    system("cls");
}

void playerDie() { // �÷��̾� ��� �� �ƾ� ���
    stopSound(L"player_death_01.wav");
    playSound(L"player_death_01.wav");
    renderImg(L"death_P20001.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20002.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20003.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20004.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20005.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20006.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20007.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20008.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20009.bmp", 435, 0, 0.72);
    Sleep(60);
    renderImg(L"death_P20010.bmp", 435, 0, 0.72);
    Sleep(250);
}

void getMouse(int* x, int* y) { // ���콺 ��ġ �޾ƿ��� �Լ�
    CIN = GetStdHandle(STD_INPUT_HANDLE); 
    GetConsoleMode(CIN, &mode);
    SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
    while (1)
    {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);
        if (rec.EventType == MOUSE_EVENT) {
            if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) { 
                int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; 
                int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y;  
                *x = mouse_x;
                *y = mouse_y; 
                break;
            }
        }
    }
}

int goal_1() { // 1��° ������ (3��° �������� �ּ� ����)
    system("cls");
    playSound(L"dialogue_start_01.wav");
    Sleep(100);

    ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image images[3] = {
        {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
        {L"benedu.bmp", 500, 100, 0.6},
        {L"s1_d.bmp", 530, 550, 1}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    getch();

    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image imagess[4] = {
        {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
        {L"benedu.bmp", 500, 100, 0.6},
        {L"s1_a1.bmp", 330, 550, 0.72},
        {L"s1_a2.bmp", 330, 650, 0.72}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 4; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = imagess;

    imageLayer.renderAll(&imageLayer);

    int check = -1;
    while (1) {
        int x, y;
        getMouse(&x, &y);
        if (x >= 21 && x <= 64) {
            if (y >= 17 && y <= 18) {
                check = 1;
                playSound(L"button_dialogue_confirm_01.wav");
                break;
            }
            if (y >= 20 && y <= 21) {
                check = 2;
                playSound(L"button_dialogue_confirm_01.wav");
                break;
            }
        }
    }

    if (check == 1) {
        imageLayer.initialize(&imageLayer); //�ʱ�ȭ

        Image imagesss[5] = {
            {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
            {L"benedu.bmp", 500, 100, 0.6},
            {L"s1_d1.bmp", 520, 550, 1}
        }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

        imageLayer.imageCount = 5; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
        imageLayer.images = imagesss;

        imageLayer.renderAll(&imageLayer);

        getch();

        //L"success0001.bmp", 0, 0, 0.72;
        playSound(L"dialogue_success_01.wav");
        Sleep(300);
        imagesss[3].fileName = L"success0001.bmp";
        imagesss[3].x = 125;
        imagesss[3].y = 250;
        imagesss[3].scale = 1;
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0002.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0003.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0004.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0005.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0006.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0007.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0008.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);

        getch();

        return 1;
    }
    else if (check == 2) {
        imageLayer.initialize(&imageLayer); //�ʱ�ȭ

        Image imagesss[3] = {
            {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
            {L"benedu.bmp", 500, 100, 0.6},
            {L"s1_d2.bmp", 520, 550, 1}
        }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

        imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
        imageLayer.images = imagesss;

        imageLayer.renderAll(&imageLayer);

        getch();

        playSound(L"bad_end_screen_01.wav");
        system("cls");
        Sleep(300);
        renderImg(L"dialogueDeathExport0001.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0002.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0003.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0004.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0005.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0006.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0007.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0008.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0009.bmp", 125, 220, 0.72);

        getch();

        return 2;
    }
}

int goal_2() { // 2��° ������ (3��° �������� �ּ� ����)
    system("cls");
    playSound(L"dialogue_start_01.wav");
    Sleep(100);

    ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image images[3] = {
        {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
        {L"dinner_guardian.bmp", 510, 110, 0.35},
        {L"s2_d.bmp", 530, 550, 1}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    getch();

    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image imagess[4] = {
        {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
        {L"dinner_guardian.bmp", 510, 110, 0.35},
        {L"s2_a1.bmp", 330, 550, 0.72},
        {L"s2_a2.bmp", 330, 650, 0.72}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 4; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = imagess;

    imageLayer.renderAll(&imageLayer);

    int check = -1;
    while (1) {
        int x, y;
        getMouse(&x, &y);
        if (x >= 21 && x <= 64) {
            if (y >= 17 && y <= 18) {
                check = 2;
                playSound(L"button_dialogue_confirm_01.wav");
                break;
            }
            if (y >= 20 && y <= 21) {
                check = 1;
                playSound(L"button_dialogue_confirm_01.wav");
                break;
            }
        }
    }

    if (check == 1) {
        imageLayer.initialize(&imageLayer); //�ʱ�ȭ

        Image imagesss[5] = {
            {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
            {L"dinner_guardian.bmp", 510, 110, 0.35},
            {L"s2_d2.bmp", 510, 550, 1}
        }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

        imageLayer.imageCount = 5; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
        imageLayer.images = imagesss;

        imageLayer.renderAll(&imageLayer);

        getch();

        //L"success0001.bmp", 0, 0, 0.72;
        playSound(L"dialogue_success_01.wav");
        Sleep(300);
        imagesss[3].fileName = L"success0001.bmp";
        imagesss[3].x = 125;
        imagesss[3].y = 250;
        imagesss[3].scale = 1;
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0002.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0003.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0004.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0005.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0006.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0007.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0008.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);

        getch();

        return 1;
    }
    else if (check == 2) {
        imageLayer.initialize(&imageLayer); //�ʱ�ȭ

        Image imagesss[3] = {
            {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
            {L"dinner_guardian.bmp", 510, 110, 0.35},
            {L"s2_d1.bmp", 510, 550, 1}
        }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

        imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
        imageLayer.images = imagesss;

        imageLayer.renderAll(&imageLayer);

        getch();

        playSound(L"bad_end_screen_01.wav");
        system("cls");
        Sleep(300);
        renderImg(L"dialogueDeathExport0001.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0002.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0003.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0004.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0005.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0006.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0007.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0008.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0009.bmp", 125, 220, 0.72);

        getch();

        return 2;
    }
}

int goal_3() { // 3��° ������
    system("cls");
    playSound(L"dialogue_start_01.wav");
    Sleep(100);

    ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image images[3] = {
        {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
        {L"hakbong.bmp", 465, 180, 1},
        {L"s3_d.bmp", 530, 550, 1}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = images;

    imageLayer.renderAll(&imageLayer);

    getch();

    imageLayer.initialize(&imageLayer); //�ʱ�ȭ

    Image imagess[4] = {
        {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
        {L"hakbong.bmp", 465, 180, 1},
        {L"s3_a1.bmp", 330, 550, 0.72},
        {L"s3_a2.bmp", 330, 650, 0.72}
    }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

    imageLayer.imageCount = 4; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
    imageLayer.images = imagess;

    imageLayer.renderAll(&imageLayer);

    int check = -1;
    while (1) { // ���콺 Ŭ�� ó��
        int x, y;
        getMouse(&x, &y);
        if (x >= 21 && x <= 64) {
            if (y >= 17 && y <= 18) {
                check = 1;
                playSound(L"button_dialogue_confirm_01.wav");
                break;
            }
            if (y >= 20 && y <= 21) {
                check = 2;
                playSound(L"button_dialogue_confirm_01.wav");
                break;
            }
        }
    }

    if (check == 1) { // �ùٸ� ����
        imageLayer.initialize(&imageLayer); //�ʱ�ȭ

        Image imagesss[5] = {
            {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
            {L"hakbong.bmp", 465, 180, 1},
            {L"s3_d1.bmp", 520, 550, 1}
        }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

        imageLayer.imageCount = 5; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
        imageLayer.images = imagesss;

        imageLayer.renderAll(&imageLayer);

        getch();

        //L"success0001.bmp", 0, 0, 0.72;
        playSound(L"dialogue_success_01.wav");
        Sleep(300);
        imagesss[3].fileName = L"success0001.bmp";
        imagesss[3].x = 125;
        imagesss[3].y = 250;
        imagesss[3].scale = 1;
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0002.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0003.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0004.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0005.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0006.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0007.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);
        imagesss[3].fileName = L"success0008.bmp";
        imageLayer.images = imagesss;
        imageLayer.renderAll(&imageLayer);
        Sleep(60);

        getch();

        return 1;
    }
    else if (check == 2) { // �߸��� ����
        imageLayer.initialize(&imageLayer); //�ʱ�ȭ

        Image imagesss[3] = {
            {L"dialogueBG_hell.bmp", 0, 100, 0.72}, //{�̹��� �̸�, ���� x��ǥ, ���� y��ǥ, ũ�� ����(���� ������ �⺻���� 16�� ��)} 
            {L"hakbong.bmp", 465, 180, 1},
            {L"s3_d2.bmp", 520, 550, 1}
        }; //�迭�� ù ���Ұ� ���� �Ʒ� �׷�����.

        imageLayer.imageCount = 3; //images �迭�� ũ�⺸�� �۰ų� ���ƾ� �Ѵ�.
        imageLayer.images = imagesss;

        imageLayer.renderAll(&imageLayer);

        getch();

        playSound(L"bad_end_screen_01.wav");
        system("cls");
        Sleep(300);
        renderImg(L"dialogueDeathExport0001.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0002.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0003.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0004.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0005.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0006.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0007.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0008.bmp", 125, 220, 0.72);
        Sleep(60);
        renderImg(L"dialogueDeathExport0009.bmp", 125, 220, 0.72);

        getch();

        return 2;
    }
}

void stage_1() { // �������� 1 (�������� 3 �ּ��� ����)
    char map[8][9] = {
        {0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 2, 1, 0},
        {0, 1, 0, 0, 3, 0, 0, 1, 0},
        {0, 1, 0, 3, 0, 3, 1, 0, 0},
        {1, 0, 0, 1, 1, 1, 1, 0, 0},
        {1, 0, 4, 0, 0, 4, 0, 1, 1},
        {1, 0, 4, 0, 4, 0, 0, 5, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    char t_map[8][9] = {
        {0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 2, 1, 0},
        {0, 1, 0, 0, 3, 0, 0, 1, 0},
        {0, 1, 0, 3, 0, 3, 1, 0, 0},
        {1, 0, 0, 1, 1, 1, 1, 0, 0},
        {1, 0, 4, 0, 0, 4, 0, 1, 1},
        {1, 0, 4, 0, 4, 0, 0, 5, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    /*
    �ԤԤԤԤԡ��
    �Ԥԡ���Կʡ�
    �ԡ�Ԥԡ�Ԥԡ�
    �ԡ�ԡ�ԡ��
    ��Ԥԡ����
    ��ԡۤԤԡۤԡ���
    ��ԡۤԡۤԤԡ١�ԤԤ�
    ����������
    */

    //makeMap(8, 9, map);

    color(White, Dgray);
    printf("��1��");
    color(Dgray, Black);
    printf("  WASD : �̵� | R : �ٽ��ϱ�\n");
    color(White, Black);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 9; j++) {
            color(White, Black);
            //gotoxy(x + j+1, y + i);
            if (map[i][j] == 0) {
                printf("��");
            }
            if (map[i][j] == 1) {
                color(Red, Black);
                printf("��");
            }
            if (map[i][j] == 2) {
                printf("��");
            }
            if (map[i][j] == 3) {
                color(Lgray, Black);
                printf("��");
            }
            if (map[i][j] == 4) {
                color(Dgray, Black);
                printf("��");
            }
            if (map[i][j] == 5) {
                color(Yellow, Black);
                printf("��");
            }
            if (map[i][j] == 6) {
                color(White, Lgray);
                printf("��");
            }
            if (map[i][j] == 7) {
                color(Dgray, Lgray);
                printf("��");
            }
            color(White, Black);
        }
        printf("\n");
    }
    int cnt = 23;
    int x = 6, y = 1;
    color(Black, White);
    if (cnt > 10) {
        printf("%3d ", cnt);
    }
    else {
        printf("%3d  ", cnt);
    }
    color(White, Black);
    
    
    
    while (1) {
        int k = getKey();
        
        while (1) {
            if (k == RESET) {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 9; j++) {
                        map[i][j] = t_map[i][j];
                    }
                }
                x = 6, y = 1;
                cnt = 23;
                showScreen();
                system("cls");
                break;
            }
            else if (k == UP) {
                if (y <= 1) break;
                if (map[y - 1][x] == 1) break;

                if (map[y - 1][x] == 0) {
                    map[y--][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y - 1][x] == 3) {
                    if (map[y - 2][x] != 0) {
                        map[y - 1][x] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y - 2][x] = 3;
                    map[y - 1][x] = 0;
                    cnt--;
                    break;
                }
                if (map[y - 1][x] == 4) {
                    if (map[y - 2][x] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y - 2][x] = 4;
                    map[y - 1][x] = 0;
                    cnt--;
                    break;
                }

                if (map[y - 1][x] == 5) {
                    int r = goal_1();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == DOWN) {
                if (y >= 6) break;
                if (map[y + 1][x] == 1) break;

                if (map[y + 1][x] == 0) {
                    map[y++][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y + 1][x] == 3) {
                    if (map[y + 2][x] != 0) {
                        map[y + 1][x] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y + 2][x] = 3;
                    map[y + 1][x] = 0;
                    cnt--;
                    break;
                }
                if (map[y + 1][x] == 4) {
                    if (map[y + 2][x] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y + 2][x] = 4;
                    map[y + 1][x] = 0;
                    cnt--;
                    break;
                }

                if (map[y + 1][x] == 5) {
                    int r = goal_1();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == LEFT) {
                if (x <= 1) break;
                if (map[y][x - 1] == 1) break;

                if (map[y][x - 1] == 0) {
                    map[y][x--] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y][x - 1] == 3) {
                    if (map[y][x - 2] != 0) {
                        map[y][x - 1] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y][x - 2] = 3;
                    map[y][x - 1] = 0;
                    cnt--;
                    break;
                }
                if (map[y][x - 1] == 4) {
                    if (map[y][x - 2] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y][x - 2] = 4;
                    map[y][x - 1] = 0;
                    cnt--;
                    break;
                }

                if (map[y][x - 1] == 5) {
                    int r = goal_1();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == RIGHT) {
                if (x >= 8) break;
                if (map[y][x + 1] == 1) break;

                if (map[y][x + 1] == 0) {
                    map[y][x++] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y][x + 1] == 3) {
                    if (map[y][x + 2] != 0) {
                        map[y][x + 1] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y][x + 2] = 3;
                    map[y][x + 1] = 0;
                    cnt--;
                    break;
                }
                if (map[y][x + 1] == 4) {
                    if (map[y][x + 2] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y][x + 2] = 4;
                    map[y][x + 1] = 0;
                    cnt--;
                    break;
                }

                if (map[y][x + 1] == 5) {
                    int r = goal_1();
                    
                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 6, y = 1;
                        cnt = 23;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else {
                break;
            }
        }

        if (cnt < 0) {
            playerDie();

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 9; j++) {
                    map[i][j] = t_map[i][j];
                }
            }
            x = 6, y = 1;
            cnt = 23;
            showScreen();
            system("cls");
        }
        
        
        //resetScreen();
        system("cls");

        color(White, Dgray);
        printf("��1��");
        color(Dgray, Black);
        printf("  WASD : �̵� | R : �ٽ��ϱ�\n");
        color(White, Black);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 9; j++) {
                color(White, Black);
                //gotoxy(x + j+1, y + i);
                if (map[i][j] == 0) {
                    printf("��");
                }
                if (map[i][j] == 1) {
                    color(Red, Black);
                    printf("��");
                }
                if (map[i][j] == 2) {
                    printf("��");
                }
                if (map[i][j] == 3) {
                    color(Lgray, Black);
                    printf("��");
                }
                if (map[i][j] == 4) {
                    color(Dgray, Black);
                    printf("��");
                }
                if (map[i][j] == 5) {
                    color(Yellow, Black);
                    printf("��");
                }
                if (map[i][j] == 6) {
                    color(White, Lgray);
                    printf("��");
                }
                if (map[i][j] == 7) {
                    color(Dgray, Lgray);
                    printf("��");
                }
                color(White, Black);
            }
            printf("\n");
        }
        color(Black, White);
        if (cnt > 10) {
            printf("%3d ", cnt);
        }
        else {
            printf("%3d  ", cnt);
        }
        color(White, Black);
    }
}

void stage_2() { // �������� 2 (�������� 3 �ּ��� ����)
    char map[8][9] = {
        {0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 1, 1},
        {0, 1, 3, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 4, 4, 4, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 2, 0, 1, 1, 0, 3, 0, 1},
        {1, 1, 1, 1, 1, 5, 0, 3, 1},
        {0, 0, 0, 0, 1, 1, 1, 1, 1}
    };
    char spike[8][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    char t_map[8][9] = {
        {0, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 1, 1},
        {0, 1, 3, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 1, 1, 4, 4, 4, 1},
        {1, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 2, 0, 1, 1, 0, 3, 0, 1},
        {1, 1, 1, 1, 1, 5, 0, 3, 1},
        {0, 0, 0, 0, 1, 1, 1, 1, 1}
    };
    /*
    �ԡ������
    �ԡ�ԤԤԤԡ���ԤԤ�
    �ԡ���ԤԤԤԡ�
    ��Ԥԡ��ۡۡۡ�
    ��Ԥԡ��ԤԤԡ�
    ��ʤԡ��ԡ�ԡ�
    ������٤ԡ��
    �ԤԤԤԡ�����
    �� : 0
    �� : 1
    �� : 2
    �� : 3
    �� : 4
    �� : 5
    --
    ���� : 6
    ���� + �� : 7
    */


    color(White, Dgray);
    printf("��2��");
    color(Dgray, Black);
    printf("  WASD : �̵� | R : �ٽ��ϱ�\n");
    color(White, Black);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 9; j++) {
            color(White, Black);
            //gotoxy(x + j+1, y + i);
            if (map[i][j] == 0) {
                if (spike[i][j] == 1) {
                    color(White, Lgray);
                }
                printf("��");
            }
            if (map[i][j] == 1) {
                color(Red, Black);
                printf("��");
            }
            if (map[i][j] == 2) {
                if (spike[i][j] == 1) {
                    color(White, Lgray);
                }
                printf("��");
            }
            if (map[i][j] == 3) {
                color(Lgray, Black);
                printf("��");
            }
            if (map[i][j] == 4) {
                if (spike[i][j] == 1) {
                    color(Dgray, Lgray);
                }
                else {
                    color(Dgray, Black);
                }
                printf("��");
            }
            if (map[i][j] == 5) {
                color(Yellow, Black);
                printf("��");
            }
            color(White, Black);
        }
        printf("\n");
    }
    int cnt = 24;
    int x = 1, y = 5;
    color(Black, White);
    if (cnt > 10) {
        printf("%3d ", cnt);
    }
    else {
        printf("%3d  ", cnt);
    }
    color(White, Black);



    while (1) {
        int k = getKey();

        while (1) {
            if (k == RESET) {
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 9; j++) {
                        map[i][j] = t_map[i][j];
                    }
                }
                x = 1, y = 5;
                cnt = 24;
                showScreen();
                system("cls");
                break;
            }
            
            if (k == UP) {
                if (y <= 1) break;
                if (map[y - 1][x] == 1) break;

                if (map[y - 1][x] == 0) {
                    map[y--][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y - 1][x] == 3) {
                    if (map[y - 2][x] != 0) {
                        map[y - 1][x] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y - 2][x] = 3;
                    map[y - 1][x] = 0;
                    cnt--;
                    break;
                }
                if (map[y - 1][x] == 4) {
                    if (map[y - 2][x] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y - 2][x] = 4;
                    map[y - 1][x] = 0;
                    cnt--;
                    break;
                }

                if (map[y - 1][x] == 5) {
                    int r = goal_2();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == DOWN) {
                if (y >= 6) break;
                if (map[y + 1][x] == 1) break;

                if (map[y + 1][x] == 0) {
                    map[y++][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y + 1][x] == 3) {
                    if (map[y + 2][x] != 0) {
                        map[y + 1][x] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y + 2][x] = 3;
                    map[y + 1][x] = 0;
                    cnt--;
                    break;
                }
                if (map[y + 1][x] == 4) {
                    if (map[y + 2][x] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y + 2][x] = 4;
                    map[y + 1][x] = 0;
                    cnt--;
                    break;
                }

                if (map[y + 1][x] == 5) {
                    int r = goal_2();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == LEFT) {
                if (x <= 1) break;
                if (map[y][x - 1] == 1) break;

                if (map[y][x - 1] == 0) {
                    map[y][x--] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y][x - 1] == 3) {
                    if (map[y][x - 2] != 0) {
                        map[y][x - 1] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y][x - 2] = 3;
                    map[y][x - 1] = 0;
                    cnt--;
                    break;
                }
                if (map[y][x - 1] == 4) {
                    if (map[y][x - 2] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y][x - 2] = 4;
                    map[y][x - 1] = 0;
                    cnt--;
                    break;
                }

                if (map[y][x - 1] == 5) {
                    int r = goal_2();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == RIGHT) {
                if (x >= 8) break;
                if (map[y][x + 1] == 1) break;

                if (map[y][x + 1] == 0) {
                    map[y][x++] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y][x + 1] == 3) {
                    if (map[y][x + 2] != 0) {
                        map[y][x + 1] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y][x + 2] = 3;
                    map[y][x + 1] = 0;
                    cnt--;
                    break;
                }
                if (map[y][x + 1] == 4) {
                    if (map[y][x + 2] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y][x + 2] = 4;
                    map[y][x + 1] = 0;
                    cnt--;
                    break;
                }

                if (map[y][x + 1] == 5) {
                    int r = goal_2();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else {
                break;
            }
        }

        if (k == UP || k == DOWN || k == LEFT || k == RIGHT) {
            if (spike[y][x] == 1) {
                stopSound(L"spikes_damage_01.wav");
                playSound(L"spikes_damage_01.wav");
                cnt--;
            }
        }

        if (cnt < 0) {
            playerDie();

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 9; j++) {
                    map[i][j] = t_map[i][j];
                }
            }
            x = 1, y = 5;
            cnt = 24;
            showScreen();
            system("cls");
        }


        //resetScreen();
        system("cls");

        color(White, Dgray);
        printf("��2��");
        color(Dgray, Black);
        printf("  WASD : �̵� | R : �ٽ��ϱ�\n");
        color(White, Black);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 9; j++) {
                color(White, Black);
                //gotoxy(x + j+1, y + i);
                if (map[i][j] == 0) {
                    if (spike[i][j] == 1) {
                        color(White, Lgray);
                    }
                    printf("��");
                }
                if (map[i][j] == 1) {
                    color(Red, Black);
                    printf("��");
                }
                if (map[i][j] == 2) {
                    if (spike[i][j] == 1) {
                        color(White, Lgray);
                    }
                    printf("��");
                }
                if (map[i][j] == 3) {
                    color(Lgray, Black);
                    printf("��");
                }
                if (map[i][j] == 4) {
                    if (spike[i][j] == 1) {
                        color(Dgray, Lgray);
                    }
                    else {
                        color(Dgray, Black);
                    }
                    printf("��");
                }
                if (map[i][j] == 5) {
                    color(Yellow, Black);
                    printf("��");
                }
                color(White, Black);
            }
            printf("\n");
        }
        color(Black, White);
        if (cnt > 10) {
            printf("%3d ", cnt);
        }
        else {
            printf("%3d  ", cnt);
        }
        color(White, Black);
    }
}

void stage_3() { // �������� 3
    char map[9][10] = { // ���� �� ����
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0, 0, 5, 0, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 6, 1, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 2, 1},
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 0, 3, 0, 0, 1, 1},
        {1, 7, 1, 0, 1, 0, 1, 0, 1, 0},
        {1, 0, 0, 0, 0, 0, 3, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
    };
    char spike[9][10] = { // ������ũ ��ġ
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    char t_map[9][10] = { // �� ���
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0, 0, 5, 0, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 6, 1, 1},
        {0, 0, 1, 0, 0, 0, 0, 0, 2, 1},
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 0, 3, 0, 0, 1, 1},
        {1, 7, 1, 0, 1, 0, 1, 0, 1, 0},
        {1, 0, 0, 0, 0, 0, 3, 0, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 0}
    };
    /*
    �ԤԤԤԡ�����
    �ԤԤԡ�ԤԤԡ١�ԤԤԤ�
    �ԤԤԡ����ǡ��ԤԤԤԤԤԤԤԤԤ�
    �Ԥԡ�ԤԤԤԤԿʡ�ԤԤԤԤ�
    �Ԥԡ�ԡ�ԡ�Ԥԡ�ԤԤԤԤԤԤ�
    ����Ԥԡ�Ԥԡ��ԤԤԤԤ�
    ��ס�ԡ�ԡ�ԡ�ԤԤԤԤԤ�
    ��ԤԤԤԤԡ�ԡ�ԤԤԤԤԤ�
    ����������ԤԤԤԤԤԤԤԤԤԤԤԤԤԤԤ�

    
    �� : 0
    �� : 1
    �� : 2
    �� : 3
    �� : 4
    �� : 5
    �� : 6
    �� : 7
    */


    color(White, Dgray);
    printf("��3��");
    color(Dgray, Black);
    printf("  WASD : �̵� | R : �ٽ��ϱ�\n");
    color(White, Black);
    for (int i = 0; i < 9; i++) { // �� ���
        for (int j = 0; j < 10; j++) {
            color(White, Black);
            if (map[i][j] == 0) {
                if (spike[i][j] == 1) {
                    color(White, Lgray);
                }
                printf("��");
            }
            if (map[i][j] == 1) {
                color(Red, Black);
                printf("��");
            }
            if (map[i][j] == 2) {
                if (spike[i][j] == 1) {
                    color(White, Lgray);
                }
                printf("��");
            }
            if (map[i][j] == 3) {
                color(Lgray, Black);
                printf("��");
            }
            if (map[i][j] == 4) {
                if (spike[i][j] == 1) {
                    color(Dgray, Lgray);
                }
                else {
                    color(Dgray, Black);
                }
                printf("��");
            }
            if (map[i][j] == 5) {
                color(Yellow, Black);
                printf("��");
            }
            if (map[i][j] == 6) {
                color(Lgray, Black);
                printf("��");
            }
            if (map[i][j] == 7) {
                color(Yellow, Black);
                printf("��");
            }
            color(White, Black);
        }
        printf("\n");
    }
    int cnt = 32;
    int x = 8, y = 3;
    int have_key = 0;
    color(Black, White);
    if (cnt > 10) {
        printf("%3d ", cnt);
    }
    else {
        printf("%3d  ", cnt);
    }
    color(White, Black);



    while (1) {
        int k = getKey();

        while (1) {
            if (k == RESET) { // RŰ�� ������ �ٽý���
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 10; j++) {
                        map[i][j] = t_map[i][j];
                    }
                }
                x = 8, y = 3;
                cnt = 32;
                have_key = 0;
                showScreen();
                system("cls");
                break;
            }

            if (k == UP) { // WŰ�� ������ ���� �̵�, ��ȣ�ۿ�
                if (y <= 1) break;
                if (map[y - 1][x] == 1) break;

                if (map[y - 1][x] == 7) {
                    map[y--][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    have_key = 1;
                    stopSound(L"key_pick_up_01.wav");
                    playSound(L"key_pick_up_01.wav");
                    break;
                }
                if (map[y - 1][x] == 6) {
                    if (have_key == 0) {
                        cnt--;
                        stopSound(L"door_closed_kick_03.wav");
                        playSound(L"door_closed_kick_03.wav");
                        break;
                    }
                    
                    stopSound(L"door_opening_01.wav");
                    playSound(L"door_opening_01.wav");
                    map[y--][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y - 1][x] == 0) {
                    map[y--][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y - 1][x] == 3) {
                    if (map[y - 2][x] != 0 || spike[y - 2][x] == 1) {
                        map[y - 1][x] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y - 2][x] = 3;
                    map[y - 1][x] = 0;
                    cnt--;
                    break;
                }
                if (map[y - 1][x] == 4) {
                    if (map[y - 2][x] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y - 2][x] = 4;
                    map[y - 1][x] = 0;
                    cnt--;
                    break;
                }

                if (map[y - 1][x] == 5) {
                    int r = goal_3();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 8, y = 3;
                        cnt = 32;
                        have_key = 0;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == DOWN) { // SŰ�� ������ �Ʒ��� �̵�, ��ȣ�ۿ�
                if (y >= 7) break;
                if (map[y + 1][x] == 1) break;

                if (map[y + 1][x] == 0) {
                    map[y++][x] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y + 1][x] == 3) {
                    if (map[y + 2][x] != 0 || spike[y + 2][x] == 1) {
                        map[y + 1][x] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y + 2][x] = 3;
                    map[y + 1][x] = 0;
                    cnt--;
                    break;
                }
                if (map[y + 1][x] == 4) {
                    if (map[y + 2][x] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y + 2][x] = 4;
                    map[y + 1][x] = 0;
                    cnt--;
                    break;
                }

                if (map[y + 1][x] == 5) {
                    int r = goal_2();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == LEFT) { // AŰ�� ������ �������� �̵�, ��ȣ�ۿ�
                if (x <= 1) break;
                if (map[y][x - 1] == 1) break;

                if (map[y][x - 1] == 0) {
                    map[y][x--] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y][x - 1] == 3) {
                    if (map[y][x - 2] != 0 || spike[y][x - 2] == 1) {
                        map[y][x - 1] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y][x - 2] = 3;
                    map[y][x - 1] = 0;
                    cnt--;
                    break;
                }
                if (map[y][x - 1] == 4) {
                    if (map[y][x - 2] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y][x - 2] = 4;
                    map[y][x - 1] = 0;
                    cnt--;
                    break;
                }

                if (map[y][x - 1] == 5) {
                    int r = goal_3();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 8, y = 3;
                        cnt = 32;
                        have_key = 0;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else if (k == RIGHT) { // DŰ�� ������ ���������� �̵�, ��ȣ�ۿ�
                if (x >= 9) break;
                if (map[y][x + 1] == 1) break;

                if (map[y][x + 1] == 0) {
                    map[y][x++] = 0;
                    map[y][x] = 2;
                    cnt--;
                    stopSound(L"move_effect.wav");
                    playSound(L"move_effect.wav");
                    break;
                }
                if (map[y][x + 1] == 3) {
                    if (map[y][x + 2] != 0 || spike[y][x + 2] == 1) {
                        map[y][x + 1] = 0;
                        cnt--;
                        stopSound(L"enemy_die_01.wav");
                        playSound(L"enemy_die_01.wav");
                        break;
                    }

                    stopSound(L"enemy_kick_01.wav");
                    playSound(L"enemy_kick_01.wav");
                    map[y][x + 2] = 3;
                    map[y][x + 1] = 0;
                    cnt--;
                    break;
                }
                if (map[y][x + 1] == 4) {
                    if (map[y][x + 2] != 0) {
                        cnt--;
                        stopSound(L"stone_kick_01.wav");
                        playSound(L"stone_kick_01.wav");
                        break;
                    }

                    stopSound(L"stone_move_01.wav");
                    playSound(L"stone_move_01.wav");
                    map[y][x + 2] = 4;
                    map[y][x + 1] = 0;
                    cnt--;
                    break;
                }

                if (map[y][x + 1] == 5) {
                    int r = goal_2();

                    if (r == 1) {
                        return;
                    }
                    else {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 9; j++) {
                                map[i][j] = t_map[i][j];
                            }
                        }
                        x = 1, y = 5;
                        cnt = 24;
                        showScreen();
                        system("cls");
                        break;
                    }
                }
            }
            else {
                break;
            }
        }

        if (k == UP || k == DOWN || k == LEFT || k == RIGHT) {
            if (spike[y][x] == 1) { // ������ũ ó��
                stopSound(L"spikes_damage_01.wav");
                playSound(L"spikes_damage_01.wav");
                cnt--;
            }
        }

        if (cnt < 0) { // �̵� Ƚ�� ��� �Ҹ�� ���� ����
            playerDie();

            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 10; j++) {
                    map[i][j] = t_map[i][j];
                }
            }
            x = 8, y = 3;
            cnt = 32;
            have_key = 0;
            showScreen();
            system("cls");
        }


        //resetScreen();
        system("cls");

        color(White, Dgray);
        printf("��3��");
        color(Dgray, Black);
        printf("  WASD : �̵� | R : �ٽ��ϱ�\n");
        color(White, Black);
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                color(White, Black);
                if (map[i][j] == 0) {
                    if (spike[i][j] == 1) {
                        color(White, Lgray);
                    }
                    printf("��");
                }
                if (map[i][j] == 1) {
                    color(Red, Black);
                    printf("��");
                }
                if (map[i][j] == 2) {
                    if (spike[i][j] == 1) {
                        color(White, Lgray);
                    }
                    printf("��");
                }
                if (map[i][j] == 3) {
                    color(Lgray, Black);
                    printf("��");
                }
                if (map[i][j] == 4) {
                    if (spike[i][j] == 1) {
                        color(Dgray, Lgray);
                    }
                    else {
                        color(Dgray, Black);
                    }
                    printf("��");
                }
                if (map[i][j] == 5) {
                    color(Yellow, Black);
                    printf("��");
                }
                if (map[i][j] == 6) {
                    color(Lgray, Black);
                    printf("��");
                }
                if (map[i][j] == 7) {
                    color(Yellow, Black);
                    printf("��");
                }
                color(White, Black);
            }
            printf("\n");
        }
        color(Black, White);
        if (cnt > 10) {
            printf("%3d ", cnt);
        }
        else {
            printf("%3d  ", cnt);
        }
        color(White, Black);
    }
}

void epilogue() { // ���ʷα�
    system("cls");
    gotoxy(17, 11);
    typingEffect(10, "����� �ᱹ �о����Ļ硤���� ��� �ι��� �����Ͽ���");
    getch();
    system("cls");
    gotoxy(22, 11);
    typingEffect(13, "���ǰ��κ��� Ż���ϰ� �Ǿ����ϴ�");
    getch();
    typingEffect(300, " . . . ?");
    getch();
    system("cls");
    gotoxy(23, 11);
    typingEffect(13, "��� ���ǰ��� ����ڸ� ���� ���̾���");
    getch();
    system("cls");
    gotoxy(20, 11);
    typingEffect(13, "����� �㹫�԰� ȥ�������� �ӿ� ���� û�߽��ϴ�");
    getch();
}

void inGame() { // ���� ���� �ý���
    while (1) {
        if (player.stage == 0) { // ���ѷα� ����
            prologue();
            stopSound(L"lobby.wav");
            showScreen();
            PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP);
            player.stage = 1;
            saveData();

        }
        if (player.stage == 1) { // �������� 1 ����
            stage_1();
            showScreen();
            player.stage = 2;
            saveData();
        }
        if (player.stage == 2) { // �������� 2 ����
            stage_2();
            showScreen();
            player.stage = 3;
            saveData();
        }
        if (player.stage == 3) { // �������� 3 ����
            stage_3();
            showScreen();
            player.stage = 4;
            saveData();
        }
        if (player.stage == 4) { // ���ʷα� ����, ���� ����
            epilogue();
            system("cls");
            Sleep(100);
            stopSound(L"screen_changer_part1_01.wav");
            stopSound(L"screen_changer_part2_01.wav");
            playSound(L"screen_changer_part1_01.wav");
            Sleep(50);
            renderImg(L"transition0002.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0003.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0004.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0005.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0006.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0007.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0008.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0009.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0010.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0011.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0012.bmp", 0, 0, 0.72);
            Sleep(50);
            renderImg(L"transition0013.bmp", 0, 0, 0.72);
            getch();
            return;
        }
    }
    
}

int main() {
    init();
    int t = menu();
    if (t == 0) { // ���� �����
        system("cls");
        makeNewGame();
        saveData();
        inGame();
    }
    else if (t == 1) { // ���� �ҷ�����
        loadData();
        showScreen();
        PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP);
        inGame();
    }
    else { // ���� ����
        system("title ���� ����");
        system("cls");
        return 0;
    }

    return;
}
