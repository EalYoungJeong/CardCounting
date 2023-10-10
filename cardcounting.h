#pragma once
#include<cstdlib>
#include<ctime>

class Simple;
class CardManage;
class Player
{
public:
	int money;		//�ʱ� �ڱ�
	int win;		//�¸� Ƚ��
	int lose;		//�й� Ƚ��
	int draw;		//���º� Ƚ��
	int card[13];	//player1�� ����ִ� ������ ī�� ����, �ε����� ī�� ����
	int count = 0;	//������ �ִ� ī�� ����
	int sumofcard;	//player1�� ����ִ� ī���� ���� �� A������� A�� 1
	int sumofcard_a; //player1�� ����ִ� ī���� ���� �� A������� A�� 11
	int finalsum;	//�����հ�
	int blackjack;
	Player();
	void betting();		//100�� ����
	void get_card(int num);	//ī�� ���� �̱�
	void final_sum();	//�����հ� ���
	void get_money(int num);//����ݾ�
	void game_end();	//���� ������ ����ִ� ī�� �ʱ�ȭ
	void SumofNum();	//ī�� ���� �� ���
	bool HitStatus(Simple* dealer, CardManage* manager);	//hit, stand �Ǻ�
};

class Simple
{
public:
	int money;		//�ʱ� �ڱ�
	int win;		//�¸� Ƚ��
	int lose;		//�й� Ƚ��
	int draw;		//���º� Ƚ��
	int card[13];	//dealer �Ǵ� player2�� ����ִ� ������ ī�� ����, �ε����� ī�� ����
	int count;		//������ �ִ� ī�� ����
	int sumofcard;	//dealer �Ǵ� player2�� ����ִ� ī���� ���� �� A������� A�� 1
	int sumofcard_a; //dealer �Ǵ� player2�� ����ִ� ī���� ���� �� A������� A�� 11
	int finalsum;	//�����հ�
	bool isdealer;	//true�̸� ���� false�̸� player2
	int firstcard;
	int blackjack;
	Simple(bool isdealer);
	void betting();		//100�� ����
	void get_card(int num);	//ī�� ���� �̱�
	void final_sum();	//�����հ� ���
	void get_money(int num);//����ݾ�
	void game_end();	//���� ������ ����ִ� ī�� �ʱ�ȭ
	void SumofNum();	//ī�� ���� �� ���
	bool HitStatus();	//hit, stand �Ǻ�
};

class CardManage
{
public:
	int card_count[13];//��ü ���� �ִ� �� ī���� ����, �ε����� ī�� ��ȣ
	int all_card_count;//��ü ���� ī�� ����
	CardManage()
	{
		for (int i = 0; i < 13; i++)//�ε��� = ī�� ����
			card_count[i] = 24;//�� ���� ���� 4�徿 * 6��
		all_card_count = 312;//��ü ī�� ���� 52 * 6 = 312
	}
	void game_setting(Simple* dealer, Player* player1, Simple* player2);//���� ����
	void give_two_cards(Simple* dealer, Player* player1, Simple* player2);//���� ���۽� ��ο��� ī�� ���� ����
	void Shuffle();//����
	void give_card(Simple* player);//dealer �Ǵ� player2���� ī�� ����
	void give_card(Player* player);//player1���� ī�� ����
};

int player1_result(Simple* dealer, Player* player1);//player1�� ���
int player2_result(Simple* dealer, Simple* player2);//player2�� ���