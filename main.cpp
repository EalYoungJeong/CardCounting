#include<iostream>
#include"cardcounting.h"
using namespace std;
int main()
{
	srand((unsigned int)time(NULL));

	CardManage* manager = new CardManage;//ī�� ������
	Player* player1 = new Player;//player1 ��ü
	Simple* dealer = new Simple(true);//dealer ��ü
	Simple* player2 = new Simple(false);//player2 ��ü

	int game;//���� Ƚ��
	int p1_result;//player1�� ���� ���
	int p2_result;//player2�� ���� ���
	cout << "Play Game : ";
	cin >> game;

	for (int i = 0; i < game; i++)//game ����ŭ �ݺ�
	{
		player1->betting();//player1 100�� ����
		player2->betting();//player2 100�� ����
		manager->game_setting(dealer, player1, player2);//shuffle + ���� �� �徿 �� �ι� �����ֱ�

		while (player1->HitStatus(dealer, manager))//player1, stand�϶����� �ݺ��̱�
		{
			manager->give_card(player1);//hit�̹Ƿ� ���� �� �̱�
		}
		while (player2->HitStatus())//player2, stand�϶����� �ݺ�
		{
			manager->give_card(player2);//hit�̹Ƿ� ���� �� �̱�
		}
		dealer->get_card(dealer->firstcard);//������ ī�� ����
		dealer->count--;//�ߺ� ����
		while (dealer->HitStatus())//dealer, stand�϶����� �ݺ�
		{
			manager->give_card(dealer);//hit�̹Ƿ� ���� �� �̱�
		}
		//���� �հ� ���
		player1->SumofNum();
		player2->SumofNum();
		dealer->SumofNum();
		//���� �����հ� ���
		player1->final_sum();
		player2->final_sum();
		dealer->final_sum();
		
		//player���� ���, 1=�¸�, 0=���º�, -1=�й�
		p1_result = player1_result(dealer, player1);//player1 ���
		p2_result = player2_result(dealer, player2);//player2 ���

		if (p1_result == -1)//player1 �й��
			player1->lose++;
		else if (p1_result == 0)//���º�
		{
			player1->draw++;
			player1->get_money(100);//���� �ݾ� ��������
		}
		else//�¸�
		{
			player1->win++;
			if (player1->count == 2 && player1->finalsum == 21)//����
			{
				player1->get_money(250);
				player1->blackjack++;
			}
			else player1->get_money(200);//�׳� �¸�
		}

		if (p2_result == -1)//player2 �й��
			player2->lose++;
		else if (p2_result == 0)//���º�
		{
			player2->draw++;
			player2->get_money(100);//���� �ݾ� ��������
		}
		else//�¸�
		{
			player2->win++;
			if (player2->count == 2 && player2->finalsum == 21)//����
			{
				player2->get_money(250);
				player2->blackjack++;
			}
			else player2->get_money(200);//�׳� �¸�
		}

		//����ִ� ī�� �ʱ�ȭ
		player1->game_end();
		player2->game_end();
		dealer->game_end();
	}

	double p1_winrate = player1->win / (double)game * 100;
	double p2_winrate = player2->win / (double)game * 100;

	//��� ���
	cout << "<<Player1>>" << endl;
	cout << "�¸� : " << player1->win << "  ���º� �� �й� : " << player1->draw + player1->lose << endl;
	cout << "�·� : " << p1_winrate << "%" << endl;
	cout << "money : " << player1->money << "  ���� : " << player1->blackjack << endl << endl;
	cout << "<<Player2>>" << endl;
	cout << "�¸� : " << player2->win << "  ���º� �� �й� : " << player2->draw + player2->lose << endl;
	cout << "�·� : " << p2_winrate << "%" << "  ���� : " << player2->blackjack << endl;
	cout << "money : " << player2->money << endl << endl;
}