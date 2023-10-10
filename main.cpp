#include<iostream>
#include"cardcounting.h"
using namespace std;
int main()
{
	srand((unsigned int)time(NULL));

	CardManage* manager = new CardManage;//카드 관리자
	Player* player1 = new Player;//player1 객체
	Simple* dealer = new Simple(true);//dealer 객체
	Simple* player2 = new Simple(false);//player2 객체

	int game;//게임 횟수
	int p1_result;//player1의 게임 결과
	int p2_result;//player2의 게임 결과
	cout << "Play Game : ";
	cin >> game;

	for (int i = 0; i < game; i++)//game 수만큼 반복
	{
		player1->betting();//player1 100원 베팅
		player2->betting();//player2 100원 베팅
		manager->game_setting(dealer, player1, player2);//shuffle + 각자 한 장씩 총 두번 나눠주기

		while (player1->HitStatus(dealer, manager))//player1, stand일때까지 반복뽑기
		{
			manager->give_card(player1);//hit이므로 한장 더 뽑기
		}
		while (player2->HitStatus())//player2, stand일때까지 반복
		{
			manager->give_card(player2);//hit이므로 한장 더 뽑기
		}
		dealer->get_card(dealer->firstcard);//숨겨진 카드 오픈
		dealer->count--;//중복 제거
		while (dealer->HitStatus())//dealer, stand일때까지 반복
		{
			manager->give_card(dealer);//hit이므로 한장 더 뽑기
		}
		//각각 합계 계산
		player1->SumofNum();
		player2->SumofNum();
		dealer->SumofNum();
		//각각 최종합계 계산
		player1->final_sum();
		player2->final_sum();
		dealer->final_sum();
		
		//player들의 결과, 1=승리, 0=무승부, -1=패배
		p1_result = player1_result(dealer, player1);//player1 결과
		p2_result = player2_result(dealer, player2);//player2 결과

		if (p1_result == -1)//player1 패배시
			player1->lose++;
		else if (p1_result == 0)//무승부
		{
			player1->draw++;
			player1->get_money(100);//베팅 금액 돌려받음
		}
		else//승리
		{
			player1->win++;
			if (player1->count == 2 && player1->finalsum == 21)//블랙잭
			{
				player1->get_money(250);
				player1->blackjack++;
			}
			else player1->get_money(200);//그냥 승리
		}

		if (p2_result == -1)//player2 패배시
			player2->lose++;
		else if (p2_result == 0)//무승부
		{
			player2->draw++;
			player2->get_money(100);//베팅 금액 돌려받음
		}
		else//승리
		{
			player2->win++;
			if (player2->count == 2 && player2->finalsum == 21)//블랙잭
			{
				player2->get_money(250);
				player2->blackjack++;
			}
			else player2->get_money(200);//그냥 승리
		}

		//들고있는 카드 초기화
		player1->game_end();
		player2->game_end();
		dealer->game_end();
	}

	double p1_winrate = player1->win / (double)game * 100;
	double p2_winrate = player2->win / (double)game * 100;

	//결과 출력
	cout << "<<Player1>>" << endl;
	cout << "승리 : " << player1->win << "  무승부 및 패배 : " << player1->draw + player1->lose << endl;
	cout << "승률 : " << p1_winrate << "%" << endl;
	cout << "money : " << player1->money << "  블랙잭 : " << player1->blackjack << endl << endl;
	cout << "<<Player2>>" << endl;
	cout << "승리 : " << player2->win << "  무승부 및 패배 : " << player2->draw + player2->lose << endl;
	cout << "승률 : " << p2_winrate << "%" << "  블랙잭 : " << player2->blackjack << endl;
	cout << "money : " << player2->money << endl << endl;
}