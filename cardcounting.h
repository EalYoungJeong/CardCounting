#pragma once
#include<cstdlib>
#include<ctime>

class Simple;
class CardManage;
class Player
{
public:
	int money;		//초기 자금
	int win;		//승리 횟수
	int lose;		//패배 횟수
	int draw;		//무승부 횟수
	int card[13];	//player1이 들고있는 각각의 카드 개수, 인덱스는 카드 숫자
	int count = 0;	//가지고 있는 카드 개수
	int sumofcard;	//player1이 들고있는 카드의 숫자 합 A뽑을경우 A는 1
	int sumofcard_a; //player1이 들고있는 카드의 숫자 합 A뽑을경우 A는 11
	int finalsum;	//최종합계
	int blackjack;
	Player();
	void betting();		//100원 베팅
	void get_card(int num);	//카드 한장 뽑기
	void final_sum();	//최종합계 계산
	void get_money(int num);//결과금액
	void game_end();	//게임 끝나면 들고있는 카드 초기화
	void SumofNum();	//카드 숫자 합 계산
	bool HitStatus(Simple* dealer, CardManage* manager);	//hit, stand 판별
};

class Simple
{
public:
	int money;		//초기 자금
	int win;		//승리 횟수
	int lose;		//패배 횟수
	int draw;		//무승부 횟수
	int card[13];	//dealer 또는 player2가 들고있는 각각의 카드 개수, 인덱스는 카드 숫자
	int count;		//가지고 있는 카드 개수
	int sumofcard;	//dealer 또는 player2가 들고있는 카드의 숫자 합 A뽑을경우 A는 1
	int sumofcard_a; //dealer 또는 player2가 들고있는 카드의 숫자 합 A뽑을경우 A는 11
	int finalsum;	//최종합계
	bool isdealer;	//true이면 딜러 false이면 player2
	int firstcard;
	int blackjack;
	Simple(bool isdealer);
	void betting();		//100원 베팅
	void get_card(int num);	//카드 한장 뽑기
	void final_sum();	//최종합계 계산
	void get_money(int num);//결과금액
	void game_end();	//게임 끝나면 들고있는 카드 초기화
	void SumofNum();	//카드 숫자 합 계산
	bool HitStatus();	//hit, stand 판별
};

class CardManage
{
public:
	int card_count[13];//전체 덱에 있는 각 카드의 개수, 인덱스는 카드 번호
	int all_card_count;//전체 덱의 카드 개수
	CardManage()
	{
		for (int i = 0; i < 13; i++)//인덱스 = 카드 숫자
			card_count[i] = 24;//한 덱에 각각 4장씩 * 6덱
		all_card_count = 312;//전체 카드 개수 52 * 6 = 312
	}
	void game_setting(Simple* dealer, Player* player1, Simple* player2);//게임 세팅
	void give_two_cards(Simple* dealer, Player* player1, Simple* player2);//게임 시작시 모두에게 카드 두장 지급
	void Shuffle();//셔플
	void give_card(Simple* player);//dealer 또는 player2에게 카드 지급
	void give_card(Player* player);//player1에게 카드 지급
};

int player1_result(Simple* dealer, Player* player1);//player1의 결과
int player2_result(Simple* dealer, Simple* player2);//player2의 결과