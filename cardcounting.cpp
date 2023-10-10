#include"cardcounting.h"

// Player
Player::Player()
{
	money = 100000;//�ʱ� �ڱ�
	win = 0;
	lose = 0;
	draw = 0;
	for (int i = 0; i < 13; i++) //ī�� ���� ��� 0���� �ʱ�ȭ
		card[i] = 0;
	count = 0;
	sumofcard = 0;
	sumofcard_a = 0;
	finalsum = 0;
	blackjack = 0;
}
void Player::betting()//100�� ����
{
	this->money -= 100; //��ü �ڱݿ��� 100�� ����
}
void Player::get_card(int num)//ī�� ���� ����, num�� ���� ī�� ����
{
	this->card[num]++;//�ش� ī�� ������ ���� +1
	count++;
}
void Player::final_sum()//�����հ� ���
{
	if (sumofcard > 21 && sumofcard_a > 21) //bust�� ���
		finalsum = sumofcard;		//�����հ�� �ƹ��ų� �������, ������ bust
	else if (sumofcard <= 21 && sumofcard_a > 21) //sumofcard_a�� but�� ���
		finalsum = sumofcard;		//�����հ�� sumofcard
	else if (sumofcard <= 21 && sumofcard_a <= 21) //�Ѵ� bust�� �ƴ� ���
	{
		if (sumofcard_a >= sumofcard)	//sumofcard_a�� �� ũ�ų� ���� ���
			finalsum = sumofcard_a;		//�����հ�� sumofcard_a
		else finalsum = sumofcard;		//�ƴ� ��� sumofcard
	}
	else finalsum = sumofcard;
	//������ ���� sumofcard_a�� 21�����̰� sumofcard�� bust�� ����ε�
	//�̶� �����հ�� sumofcard_a
}
void Player::get_money(int num)//��� �ݾ� ����
{
	this->money += num;
}
void Player::game_end()//���� ������ ��� �ִ� ī�� �ʱ�ȭ
{
	for (int i = 0; i < 13; i++)
		card[i] = 0;
	count = 0;
}
void Player::SumofNum()//player1�� ����ִ� ī�� ���� ��
{
	sumofcard = 0;
	sumofcard_a = 0;
	bool a_flag = true;
	int card_num;
	for (int i = 0; i < 13; i++)//�ε����� ī�� ����-1
	{
		if (i >= 9)//ī�� ���ڰ� 10, J, Q, K�ΰ��
			card_num = 10;//ī�� ���ڴ� 10���� ����
		else card_num = i+1;//�ƴϸ� ī����� = �ε���+1
		sumofcard += card[i] * card_num;//ī�� ���� * ī�� ����
	}
	for (int i = 0; i < 13; i++)//�ε����� ī�� ����
	{
		if (i >= 9)//ī�� ���ڰ� 10, J, Q, K�ΰ��
			card_num = 10;//ī�� ���ڴ� 10���� ����
		else card_num = i+1;//�ƴϸ� ī����� = �ε���+1
		if (i == 0)//A�� ���
		{
			if (card[0] == 1)//A�� ������ ���
				sumofcard_a += 11;//A = 11
			if (card[0] >= 2)//A�� ���� �̻��ϰ��
			{
				sumofcard_a += 11;//ó�� A�� 11
				sumofcard_a += card[0] - 1;//�������� 1
			}
		}
		else sumofcard_a += card[i] * card_num;//ī�� ���� * ī�� ����
	}
}
bool Player::HitStatus(Simple* dealer, CardManage* manager)//player1�� hit, stand �Ǻ�
{
	double mybust;//player1�� hit���� bust�� Ȯ��
	double dealerbust;//dealer�� bust�� Ȯ��
	int dealersum = 0;
	int flag;
	int flag_sum = 0;
	this->SumofNum();
	if (this->sumofcard > 21 && this->sumofcard_a > 21)//bust �� ���
		return false;//stand

	if (this->sumofcard >= 17 && this->sumofcard <= 21)//A=1, player1 ī������ 17�� ���� ���
		return false;//stand
	if (this->sumofcard_a >= 17 && this->sumofcard_a <= 21)//A=11, player1 ī������ 17�� ���� ���
		return false;//stand

	for (int i = 0; i < 13; i++)//dealer�� ī�� �� ���
	{
		if (i == 0) 
			dealersum += dealer->card[0]*11;//open�� ������ A�� ���

		else dealersum += dealer->card[i] * (i + 1);//�� �������� ���
	}
	dealersum += 10;//ù��° ī�� 10�̶�� ����

	if(dealersum<17)//������ 17���� ���� ��
	{
		if (this->card[0] == 0)//player1�� A�� ���� ��Ȳ�� ��
		{
			//hit ���� �� bust Ȯ���� ����
			flag = 21 - sumofcard;
			flag_sum = 0;
			for (int i = flag; i < 13; i++)
				flag_sum += manager->card_count[i];
			mybust = flag_sum / (double)manager->all_card_count;

			if (mybust > 0.5)//50���� �̻��̸� hit�� ���ϴ°� ����
				return false;//player1�� bust �ϸ� ������ �й��ϱ⶧��
			else
			{//50���� �Ʒ��� ������ bust Ȯ���� ���ؼ� ��
				flag = 21 - dealersum;
				flag_sum = 0;
				for (int i = flag; i < 13; i++)
					flag_sum += manager->card_count[i];
				dealerbust = flag_sum / (double)manager->all_card_count;

				if (mybust > dealerbust)//player1 bust Ȯ���� �� ���� ���
					return false;//stand
				else return true;//dealer bust Ȯ���� ������ hit
			}
		}
		else//A�� �̾��� ��
		{
			if (this->sumofcard_a > 21)//A=11�� ���� bust�϶�
			{//A=1�϶��� ����Ͽ� hit �� bust Ȯ�� ���
				flag = 21 - sumofcard;
				flag_sum = 0;
				for (int i = flag; i < 13; i++)
					flag_sum += manager->card_count[i];
				mybust = flag_sum / (double)manager->all_card_count;

				if (mybust > 0.5)//50���� �̻��̸� hit ���ϴ°� ����
					return false;//bust�ϸ� ������ �й��ϱ� ����
				else
				{//50���� �Ʒ��� ������ bust Ȯ���� ���ؼ� ��
					flag = 21 - dealersum;
					flag_sum = 0;
					for (int i = flag; i < 13; i++)
						flag_sum += manager->card_count[i];
					dealerbust = flag_sum / (double)manager->all_card_count;

					if (mybust > dealerbust)//player1 bust Ȯ���� ������
						return false;//�״�� stand
					else return true;//dealer bust Ȯ���� ������ hit
				}
			}
			else if (this->sumofcard_a <= 21 && this->sumofcard_a >= 17)
				return false;//�̹� �����ǿ� �������Ƿ� stand
			else
			{//A=11�� 17���� ���� ��� A=11�϶� hit �� bust Ȯ�� ���
				flag = 21 - sumofcard_a;
				flag_sum = 0;
				for (int i = flag; i < 13; i++)
					flag_sum += manager->card_count[i];
				mybust = flag_sum / (double)manager->all_card_count;

				if (mybust > 0.5)//50���� �̻��̸� hit ���ϴ°� ����
					return false;//bust�ϸ� ������ �й��ϱ� ����
				else
				{//50���� �Ʒ��� ������ bust Ȯ���� ���ؼ� ���ؾ��ϳ�
				 //��¥�� �Ѵ� 17�Ʒ��� ��Ȳ���� ī�� ���� ���� ���� bust Ȯ���� �� ŭ
					if (sumofcard_a > dealersum)
						return false;//stand�ϰ� dealer�� bust �ϱ⸦ ���
					else return true;//�ƴϸ� hit
				}
			}
		}
	}
	else//dealer�� 17���� Ŀ�� stand ����
	{
		if (card[0] == 0)//A�� ���� �ʾ��� ��
		{
			if (this->sumofcard <= dealersum)//dealer�� �� ũ�ų� ���� ���
				return true;	//hit
			else return false;//dealer���� �� ũ�� stand 
		}
		else//A�� �̾��� ��
		{
			if (this->sumofcard_a > 21)//A=11�� bust�� ���
			{
				if (this->sumofcard > dealersum)//�̹� dealersum�� 17�̻��̹Ƿ�
					return false;//bust�̴� �������� ���̴� stand
				else return true;//�ƴϸ� ������ �����ۿ� �����Ƿ� hit
			}
			else if (this->sumofcard_a >= 17 && this->sumofcard_a <= 21)
				return false;//A=11�϶� ���� 17�̻� bust �ƴ� ��� �״�� stand
			else
				return true;//�� �ܴ� sumofcard_a�� 17���� �����Ƿ� ������ �����ۿ� ���� �׷��� hit

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
	for (int i = 0; i < 13; i++)	//ī�� ���� ��� 0���� �ʱ�ȭ
		card[i] = 0;
	count = 0;
	sumofcard = 0;
	sumofcard_a = 0;
	finalsum = 0;
	firstcard = 0;
	this->isdealer = isdealer;
	blackjack = 0;
}
void Simple::betting()//100�� ����
{
	this->money -= 100;//��ü �ڱݿ��� 100�� ����
}
void Simple::get_card(int num)// ī�� ���� ����, num�� ���� ī�� ����
{
	this->card[num]++;
	count++;
}
void Simple::final_sum()//�����հ� ���
{
	if (sumofcard > 21 && sumofcard_a > 21) //bust�� ���
		finalsum = sumofcard;		//�����հ�� �ƹ��ų� �������, ������ bust
	else if (sumofcard <= 21 && sumofcard_a > 21) //sumofcard_a�� but�� ���
		finalsum = sumofcard;		//�����հ�� sumofcard
	else if (sumofcard <= 21 && sumofcard_a <= 21) //�Ѵ� bust�� �ƴ� ���
	{
		if (sumofcard_a >= sumofcard)	//sumofcard_a�� �� ũ�ų� ���� ���
			finalsum = sumofcard_a;		//�����հ�� sumofcard_a
		else finalsum = sumofcard;		//�ƴ� ��� sumofcard
	}
	else finalsum = sumofcard;
	//������ ���� sumofcard_a�� 21�����̰� sumofcard�� bust�� ����ε�
	//�̶� �����հ�� sumofcard_a
}
void Simple::get_money(int num)//��� �ݾ� ����
{
	this->money += num;
}
void Simple::game_end()//���� ������ ��� �ִ� ī�� �ʱ�ȭ
{
	for (int i = 0; i < 13; i++)
		card[i] = 0;
	count = 0;
}
void Simple::SumofNum()//dealer �Ǵ� player2�� ��� �ִ� ī�� ���� ��
{
	sumofcard = 0;
	sumofcard_a = 0;
	bool a_flag = true;
	int card_num;
	for (int i = 0; i < 13; i++)//�ε����� ī�� ����-1
	{
		if (i >= 9)//ī�� ���ڰ� 10, J, Q, K�ΰ��
			card_num = 10;//ī�� ���ڴ� 10���� ����
		else card_num = i+1;//�ƴϸ� ī����� = �ε���+1
		sumofcard += card[i] * card_num;//ī�� ���� * ī�� ����
	}
	for (int i = 0; i < 13; i++)//�ε����� ī�� ����-1
	{
		if (i >= 9)//ī�� ���ڰ� 10, J, Q, K�ΰ��
			card_num = 10;//ī�� ���ڴ� 10���� ����
		else card_num = i+1;//�ƴϸ� ī����� = �ε���+1
		if (i == 0)//A�� ���
		{
			if (card[0] == 1)//A�� ������ ���
				sumofcard_a += 11;//A = 11
			if (card[0] >= 2)//A�� ���� �̻��ϰ��
			{
				sumofcard_a += 11;//ó�� A�� 11
				sumofcard_a += card[0] - 1;//�������� 1
			}
		}
		else sumofcard_a += card[i] * card_num;//ī�� ���� * ī�� ����
	}
}
bool Simple::HitStatus()//dealer�� player2�� hit, stand �Ǻ�
{
	SumofNum();
	if (card[0] == 0)//A�� ���� ���� ���
	{
		if (sumofcard < 17)//17�̸��� ��� hit
			return true;
		else               //17�̻��� ��� stand
			return false;
	}
	else//A�� ���� ���
	{
		if (sumofcard_a < 17)//A=11 ���� ����� ��쿡�� 17�̸��� ��� hit
			return true;
		else if (sumofcard_a > 21)//A=11 ���� ��������� bust, A=1�� ��������� 17�̸��� ��� hit
		{
			if (sumofcard < 17) return true;
			else return false;
		}
		else return false;//A=11�϶� �հ谡 17���� 21 ����, stand
	}
}

// CardManage
void CardManage::game_setting(Simple* dealer, Player* player1, Simple* player2)//���� ����
{
	if (all_card_count < 312 * 0.2)//ī�� 80% �̻� ���� ����
		Shuffle();
	give_two_cards(dealer, player1, player2);//��ο��� ī�� ���徿 ����

}
void CardManage::give_two_cards(Simple* dealer, Player* player1, Simple* player2)//���� ���� �� ��ο��� ī�� ���� ����
{
	int card_num;
	for (int i = 0; i < 2; i++)//��ο��� ī�� ���徿 �� �ι� ����
	{
		card_num = rand() % 13;//ī�� ���� �̱�
		while (1)
		{
			if (card_count[card_num] == 0)	//�̹� ���� ���� ���ڰ� ���ð��
				card_num = rand() % 13;		//�ٽ� ���� �̱�
			else
			{
				card_count[card_num]--;		//������ ���� ī�� ������ ���� ����
				all_card_count--;			//��ü ī�� �� ����
				if (i == 0)//ù��° ī���� ���
				{
					dealer->firstcard = card_num;//ī��迭�� ���� �ʰ� �ٸ� ������ ���� ����
					dealer->count++;//ī�� �� ����
				}
				else dealer->get_card(card_num);	//�������� ī�� ����
				break;		//���ѹݺ��� Ż��
			}
		}
		card_num = rand() % 13;//ī�� ���� �̱�
		while (1)
		{
			if (card_count[card_num] == 0)	//�̹� ���� ���� ���ڰ� ���ð��
				card_num = rand() % 13;		//�ٽ� ���� �̱�
			else
			{
				card_count[card_num]--;		//������ ���� ī�� ������ ���� ����
				all_card_count--;			//��ü ī�� �� ����
				player1->get_card(card_num);	//player1 ���� ī�� ����
				break;		//���ѹݺ��� Ż��
			}
		}
		card_num = rand() % 13;//ī�� ���� �̱�
		while (1)
		{
			if (card_count[card_num] == 0)	//�̹� ���� ���� ���ڰ� ���ð��
				card_num = rand() % 13;		//�ٽ� ���� �̱�
			else
			{
				card_count[card_num]--;		//������ ���� ī�� ������ ���� ����
				all_card_count--;			//��ü ī�� �� ����
				player2->get_card(card_num);	//player2 ���� ī�� ����
				break;		//���ѹݺ��� Ż��
			}
		}
	}
}
void CardManage::Shuffle()//����
{
	for (int i = 0; i < 13; i++)//��ü ���� ���� ī�� ���� ���� �ʱ�ȭ
		card_count[i] = 24;//�� ���� ���� 4�徿 * 6��
	all_card_count = 312;//��ü ī�� ���� 52 * 6 = 312
}
void CardManage::give_card(Simple* player)
{
	int card_num = rand() % 13;//ī�� ���� �̱�
	while (1)
	{
		if (card_count[card_num] == 0)	//�̹� ���� ���� ���ڰ� ���ð��
			card_num = rand() % 13;		//�ٽ� ���� �̱�
		else
		{
			card_count[card_num]--;		//������ ���� ī�� ������ ���� ����
			all_card_count--;			//��ü ī�� �� ����
			player->get_card(card_num);	//dealer �Ǵ� player2 ���� ī�� ����
			break;		//���ѹݺ��� Ż��
		}
	}
}
void CardManage::give_card(Player* player)
{
	int card_num = rand() % 13;//ī�� ���� �̱�
	while (1)
	{
		if (card_count[card_num] == 0)	//�̹� ���� ���� ���ڰ� ���ð��
			card_num = rand() % 13;		//�ٽ� ���� �̱�
		else
		{
			card_count[card_num]--;		//������ ���� ī�� ������ ���� ����
			all_card_count--;			//��ü ī�� �� ����
			player->get_card(card_num);	//player1���� ī�� ����
			break;		//���ѹݺ��� Ż��
		}
	}
}

int player1_result(Simple* dealer, Player* player1)//player1�� ��� 1=�¸�, 0=���º�, -1=�й�
{
	if (player1->finalsum > 21)//player1�� �����հ谡 bust�ΰ��
		return -1;//������ �й�

	if (dealer->finalsum > 21)//dealr�� �����հ谡 bust�ΰ��
		return 1;//������ �¸�, �̹� player1�� bust�� �ƴϱ� ����

	if (dealer->finalsum > player1->finalsum)
		return -1;//dealer�� ���� �� ũ�� �й�
	else if (dealer->finalsum == player1->finalsum)
		return 0;//������ ���º�
	else return 1;//�� �� �¸�
}
int player2_result(Simple* dealer, Simple* player2)//player2�� ���
{
	if (player2->finalsum > 21)//player2�� �����հ谡 bust�ΰ��
		return -1;//������ �й�

	if (dealer->finalsum > 21)//dealr�� �����հ谡 bust�ΰ��
		return 1;//������ �¸�, �̹� player2�� bust�� �ƴϱ� ����

	if (dealer->finalsum > player2->finalsum)
		return -1;//dealer�� ���� �� ũ�� �й�
	else if (dealer->finalsum == player2->finalsum)
		return 0;//������ ���º�
	else return 1;//�� �� �¸�
}