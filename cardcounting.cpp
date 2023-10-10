#include"cardcounting.h"

// Player
Player::Player()
{
	money = 100000;//초기 자금
	win = 0;
	lose = 0;
	draw = 0;
	for (int i = 0; i < 13; i++) //카드 개수 모두 0으로 초기화
		card[i] = 0;
	count = 0;
	sumofcard = 0;
	sumofcard_a = 0;
	finalsum = 0;
	blackjack = 0;
}
void Player::betting()//100원 베팅
{
	this->money -= 100; //전체 자금에서 100원 차감
}
void Player::get_card(int num)//카드 한장 뽑음, num은 뽑은 카드 숫자
{
	this->card[num]++;//해당 카드 숫자의 개수 +1
	count++;
}
void Player::final_sum()//최종합계 계산
{
	if (sumofcard > 21 && sumofcard_a > 21) //bust인 경우
		finalsum = sumofcard;		//최종합계는 아무거나 상관없음, 무조건 bust
	else if (sumofcard <= 21 && sumofcard_a > 21) //sumofcard_a만 but인 경우
		finalsum = sumofcard;		//최종합계는 sumofcard
	else if (sumofcard <= 21 && sumofcard_a <= 21) //둘다 bust가 아닌 경우
	{
		if (sumofcard_a >= sumofcard)	//sumofcard_a가 더 크거나 같을 경우
			finalsum = sumofcard_a;		//최종합계는 sumofcard_a
		else finalsum = sumofcard;		//아닐 경우 sumofcard
	}
	else finalsum = sumofcard;
	//나머지 경우는 sumofcard_a가 21이하이고 sumofcard가 bust인 경우인데
	//이때 최종합계는 sumofcard_a
}
void Player::get_money(int num)//결과 금액 수령
{
	this->money += num;
}
void Player::game_end()//게임 끝나면 들고 있는 카드 초기화
{
	for (int i = 0; i < 13; i++)
		card[i] = 0;
	count = 0;
}
void Player::SumofNum()//player1이 들고있는 카드 숫자 합
{
	sumofcard = 0;
	sumofcard_a = 0;
	bool a_flag = true;
	int card_num;
	for (int i = 0; i < 13; i++)//인덱스는 카드 숫자-1
	{
		if (i >= 9)//카드 숫자가 10, J, Q, K인경우
			card_num = 10;//카드 숫자는 10으로 통일
		else card_num = i+1;//아니면 카드숫자 = 인덱스+1
		sumofcard += card[i] * card_num;//카드 개수 * 카드 숫자
	}
	for (int i = 0; i < 13; i++)//인덱스는 카드 숫자
	{
		if (i >= 9)//카드 숫자가 10, J, Q, K인경우
			card_num = 10;//카드 숫자는 10으로 통일
		else card_num = i+1;//아니면 카드숫자 = 인덱스+1
		if (i == 0)//A의 경우
		{
			if (card[0] == 1)//A가 한장일 경우
				sumofcard_a += 11;//A = 11
			if (card[0] >= 2)//A가 두장 이상일경우
			{
				sumofcard_a += 11;//처음 A는 11
				sumofcard_a += card[0] - 1;//나머지는 1
			}
		}
		else sumofcard_a += card[i] * card_num;//카드 개수 * 카드 숫자
	}
}
bool Player::HitStatus(Simple* dealer, CardManage* manager)//player1의 hit, stand 판별
{
	double mybust;//player1이 hit에서 bust할 확률
	double dealerbust;//dealer가 bust할 확률
	int dealersum = 0;
	int flag;
	int flag_sum = 0;
	this->SumofNum();
	if (this->sumofcard > 21 && this->sumofcard_a > 21)//bust 인 경우
		return false;//stand

	if (this->sumofcard >= 17 && this->sumofcard <= 21)//A=1, player1 카드합이 17이 넘은 경우
		return false;//stand
	if (this->sumofcard_a >= 17 && this->sumofcard_a <= 21)//A=11, player1 카드합이 17이 넘은 경우
		return false;//stand

	for (int i = 0; i < 13; i++)//dealer의 카드 합 계산
	{
		if (i == 0) 
			dealersum += dealer->card[0]*11;//open된 한장이 A일 경우

		else dealersum += dealer->card[i] * (i + 1);//그 나머지일 경우
	}
	dealersum += 10;//첫번째 카드 10이라고 가정

	if(dealersum<17)//딜러가 17보다 작을 때
	{
		if (this->card[0] == 0)//player1이 A가 없는 상황일 때
		{
			//hit 했을 때 bust 확률을 구함
			flag = 21 - sumofcard;
			flag_sum = 0;
			for (int i = flag; i < 13; i++)
				flag_sum += manager->card_count[i];
			mybust = flag_sum / (double)manager->all_card_count;

			if (mybust > 0.5)//50프로 이상이면 hit를 안하는게 나음
				return false;//player1이 bust 하면 무조건 패배하기때문
			else
			{//50프로 아래면 딜러의 bust 확률을 구해서 비교
				flag = 21 - dealersum;
				flag_sum = 0;
				for (int i = flag; i < 13; i++)
					flag_sum += manager->card_count[i];
				dealerbust = flag_sum / (double)manager->all_card_count;

				if (mybust > dealerbust)//player1 bust 확률이 더 높은 경우
					return false;//stand
				else return true;//dealer bust 확률이 높으면 hit
			}
		}
		else//A를 뽑았을 때
		{
			if (this->sumofcard_a > 21)//A=11한 값이 bust일때
			{//A=1일때로 계산하여 hit 시 bust 확률 계산
				flag = 21 - sumofcard;
				flag_sum = 0;
				for (int i = flag; i < 13; i++)
					flag_sum += manager->card_count[i];
				mybust = flag_sum / (double)manager->all_card_count;

				if (mybust > 0.5)//50프로 이상이면 hit 안하는게 나음
					return false;//bust하면 무조건 패배하기 때문
				else
				{//50프로 아래면 딜러의 bust 확률을 구해서 비교
					flag = 21 - dealersum;
					flag_sum = 0;
					for (int i = flag; i < 13; i++)
						flag_sum += manager->card_count[i];
					dealerbust = flag_sum / (double)manager->all_card_count;

					if (mybust > dealerbust)//player1 bust 확률이 높으면
						return false;//그대로 stand
					else return true;//dealer bust 확률이 높으면 hit
				}
			}
			else if (this->sumofcard_a <= 21 && this->sumofcard_a >= 17)
				return false;//이미 안정권에 들어왔으므로 stand
			else
			{//A=11이 17보다 작은 경우 A=11일때 hit 시 bust 확률 계산
				flag = 21 - sumofcard_a;
				flag_sum = 0;
				for (int i = flag; i < 13; i++)
					flag_sum += manager->card_count[i];
				mybust = flag_sum / (double)manager->all_card_count;

				if (mybust > 0.5)//50프로 이상이면 hit 안하는게 나음
					return false;//bust하면 무조건 패배하기 때문
				else
				{//50프로 아래면 딜러의 bust 확률을 구해서 비교해야하나
				 //어짜피 둘다 17아래인 상황에서 카드 합이 높은 쪽이 bust 확률이 더 큼
					if (sumofcard_a > dealersum)
						return false;//stand하고 dealer가 bust 하기를 기대
					else return true;//아니면 hit
				}
			}
		}
	}
	else//dealer가 17보다 커서 stand 예상
	{
		if (card[0] == 0)//A를 뽑지 않았을 때
		{
			if (this->sumofcard <= dealersum)//dealer가 더 크거나 같은 경우
				return true;	//hit
			else return false;//dealer보다 더 크면 stand 
		}
		else//A를 뽑았을 때
		{
			if (this->sumofcard_a > 21)//A=11이 bust인 경우
			{
				if (this->sumofcard > dealersum)//이미 dealersum이 17이상이므로
					return false;//bust이던 정상적인 값이던 stand
				else return true;//아니면 무조건 질수밖에 없으므로 hit
			}
			else if (this->sumofcard_a >= 17 && this->sumofcard_a <= 21)
				return false;//A=11일때 값이 17이상 bust 아닐 경우 그대로 stand
			else
				return true;//그 외는 sumofcard_a가 17보다 작으므로 무조건 질수밖에 없음 그래서 hit

		}
	}
}


// Simple
Simple::Simple(bool isdealer)
{
	money = 100000;
	win = 0;
	lose = 0;
	draw = 0;
	for (int i = 0; i < 13; i++)	//카드 개수 모두 0으로 초기화
		card[i] = 0;
	count = 0;
	sumofcard = 0;
	sumofcard_a = 0;
	finalsum = 0;
	firstcard = 0;
	this->isdealer = isdealer;
	blackjack = 0;
}
void Simple::betting()//100원 베팅
{
	this->money -= 100;//전체 자금에서 100원 차감
}
void Simple::get_card(int num)// 카드 한장 뽑음, num은 뽑은 카드 숫자
{
	this->card[num]++;
	count++;
}
void Simple::final_sum()//최종합계 계산
{
	if (sumofcard > 21 && sumofcard_a > 21) //bust인 경우
		finalsum = sumofcard;		//최종합계는 아무거나 상관없음, 무조건 bust
	else if (sumofcard <= 21 && sumofcard_a > 21) //sumofcard_a만 but인 경우
		finalsum = sumofcard;		//최종합계는 sumofcard
	else if (sumofcard <= 21 && sumofcard_a <= 21) //둘다 bust가 아닌 경우
	{
		if (sumofcard_a >= sumofcard)	//sumofcard_a가 더 크거나 같을 경우
			finalsum = sumofcard_a;		//최종합계는 sumofcard_a
		else finalsum = sumofcard;		//아닐 경우 sumofcard
	}
	else finalsum = sumofcard;
	//나머지 경우는 sumofcard_a가 21이하이고 sumofcard가 bust인 경우인데
	//이때 최종합계는 sumofcard_a
}
void Simple::get_money(int num)//결과 금액 수령
{
	this->money += num;
}
void Simple::game_end()//게임 끝나면 들고 있는 카드 초기화
{
	for (int i = 0; i < 13; i++)
		card[i] = 0;
	count = 0;
}
void Simple::SumofNum()//dealer 또는 player2가 들고 있는 카드 숫자 합
{
	sumofcard = 0;
	sumofcard_a = 0;
	bool a_flag = true;
	int card_num;
	for (int i = 0; i < 13; i++)//인덱스는 카드 숫자-1
	{
		if (i >= 9)//카드 숫자가 10, J, Q, K인경우
			card_num = 10;//카드 숫자는 10으로 통일
		else card_num = i+1;//아니면 카드숫자 = 인덱스+1
		sumofcard += card[i] * card_num;//카드 개수 * 카드 숫자
	}
	for (int i = 0; i < 13; i++)//인덱스는 카드 숫자-1
	{
		if (i >= 9)//카드 숫자가 10, J, Q, K인경우
			card_num = 10;//카드 숫자는 10으로 통일
		else card_num = i+1;//아니면 카드숫자 = 인덱스+1
		if (i == 0)//A의 경우
		{
			if (card[0] == 1)//A가 한장일 경우
				sumofcard_a += 11;//A = 11
			if (card[0] >= 2)//A가 두장 이상일경우
			{
				sumofcard_a += 11;//처음 A는 11
				sumofcard_a += card[0] - 1;//나머지는 1
			}
		}
		else sumofcard_a += card[i] * card_num;//카드 개수 * 카드 숫자
	}
}
bool Simple::HitStatus()//dealer와 player2의 hit, stand 판별
{
	SumofNum();
	if (card[0] == 0)//A를 뽑지 않은 경우
	{
		if (sumofcard < 17)//17미만일 경우 hit
			return true;
		else               //17이상일 경우 stand
			return false;
	}
	else//A를 뽑은 경우
	{
		if (sumofcard_a < 17)//A=11 으로 계산한 경우에도 17미만일 경우 hit
			return true;
		else if (sumofcard_a > 21)//A=11 으로 계산했을때 bust, A=1로 계산했을때 17미만일 경우 hit
		{
			if (sumofcard < 17) return true;
			else return false;
		}
		else return false;//A=11일때 합계가 17에서 21 사이, stand
	}
}

// CardManage
void CardManage::game_setting(Simple* dealer, Player* player1, Simple* player2)//게임 세팅
{
	if (all_card_count < 312 * 0.2)//카드 80% 이상 사용시 셔플
		Shuffle();
	give_two_cards(dealer, player1, player2);//모두에게 카드 두장씩 지급

}
void CardManage::give_two_cards(Simple* dealer, Player* player1, Simple* player2)//게임 시작 시 모두에게 카드 두장 지급
{
	int card_num;
	for (int i = 0; i < 2; i++)//모두에게 카드 한장씩 총 두번 지급
	{
		card_num = rand() % 13;//카드 한장 뽑기
		while (1)
		{
			if (card_count[card_num] == 0)	//이미 덱에 없는 숫자가 나올경우
				card_num = rand() % 13;		//다시 한장 뽑기
			else
			{
				card_count[card_num]--;		//덱에서 뽑은 카드 숫자의 개수 감소
				all_card_count--;			//전체 카드 수 감소
				if (i == 0)//첫번째 카드일 경우
				{
					dealer->firstcard = card_num;//카드배열에 넣지 않고 다른 변수에 따로 저장
					dealer->count++;//카드 수 증가
				}
				else dealer->get_card(card_num);	//딜러에게 카드 지급
				break;		//무한반복문 탈출
			}
		}
		card_num = rand() % 13;//카드 한장 뽑기
		while (1)
		{
			if (card_count[card_num] == 0)	//이미 덱에 없는 숫자가 나올경우
				card_num = rand() % 13;		//다시 한장 뽑기
			else
			{
				card_count[card_num]--;		//덱에서 뽑은 카드 숫자의 개수 감소
				all_card_count--;			//전체 카드 수 감소
				player1->get_card(card_num);	//player1 에게 카드 지급
				break;		//무한반복문 탈출
			}
		}
		card_num = rand() % 13;//카드 한장 뽑기
		while (1)
		{
			if (card_count[card_num] == 0)	//이미 덱에 없는 숫자가 나올경우
				card_num = rand() % 13;		//다시 한장 뽑기
			else
			{
				card_count[card_num]--;		//덱에서 뽑은 카드 숫자의 개수 감소
				all_card_count--;			//전체 카드 수 감소
				player2->get_card(card_num);	//player2 에게 카드 지급
				break;		//무한반복문 탈출
			}
		}
	}
}
void CardManage::Shuffle()//셔플
{
	for (int i = 0; i < 13; i++)//전체 덱의 각각 카드 숫자 개수 초기화
		card_count[i] = 24;//한 덱에 각각 4장씩 * 6덱
	all_card_count = 312;//전체 카드 개수 52 * 6 = 312
}
void CardManage::give_card(Simple* player)
{
	int card_num = rand() % 13;//카드 한장 뽑기
	while (1)
	{
		if (card_count[card_num] == 0)	//이미 덱에 없는 숫자가 나올경우
			card_num = rand() % 13;		//다시 한장 뽑기
		else
		{
			card_count[card_num]--;		//덱에서 뽑은 카드 숫자의 개수 감소
			all_card_count--;			//전체 카드 수 감소
			player->get_card(card_num);	//dealer 또는 player2 에게 카드 지급
			break;		//무한반복문 탈출
		}
	}
}
void CardManage::give_card(Player* player)
{
	int card_num = rand() % 13;//카드 한장 뽑기
	while (1)
	{
		if (card_count[card_num] == 0)	//이미 덱에 없는 숫자가 나올경우
			card_num = rand() % 13;		//다시 한장 뽑기
		else
		{
			card_count[card_num]--;		//덱에서 뽑은 카드 숫자의 개수 감소
			all_card_count--;			//전체 카드 수 감소
			player->get_card(card_num);	//player1에게 카드 지급
			break;		//무한반복문 탈출
		}
	}
}

int player1_result(Simple* dealer, Player* player1)//player1의 결과 1=승리, 0=무승부, -1=패배
{
	if (player1->finalsum > 21)//player1의 최종합계가 bust인경우
		return -1;//무조건 패배

	if (dealer->finalsum > 21)//dealr의 최종합계가 bust인경우
		return 1;//무조건 승리, 이미 player1은 bust가 아니기 때문

	if (dealer->finalsum > player1->finalsum)
		return -1;//dealer의 합이 더 크면 패배
	else if (dealer->finalsum == player1->finalsum)
		return 0;//같으면 무승부
	else return 1;//그 외 승리
}
int player2_result(Simple* dealer, Simple* player2)//player2의 결과
{
	if (player2->finalsum > 21)//player2의 최종합계가 bust인경우
		return -1;//무조건 패배

	if (dealer->finalsum > 21)//dealr의 최종합계가 bust인경우
		return 1;//무조건 승리, 이미 player2는 bust가 아니기 때문

	if (dealer->finalsum > player2->finalsum)
		return -1;//dealer의 합이 더 크면 패배
	else if (dealer->finalsum == player2->finalsum)
		return 0;//같으면 무승부
	else return 1;//그 외 승리
}