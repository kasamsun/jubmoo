//============================================================
//           Algorithm for Jubmoo
//		Name		: SuperPig
//		Author	: TOM
//		Release	: 9/9/96
//		Rating	: **
//============================================================
#include "jmalgr.h"

extern int MyPlayerID;
extern int MyVar[4][10];		// variable , constant flag to store some knownledge

int SuperPig(void)
{
	///////////// change to CHOUAN mode ///////////////
	if ( CountAllLeaded() == 0 )
		if ( HandMoreCard(HEART_10)==4 )
			MyVar[MyPlayerID][0] = 4;
	if ( IsCardOnGame(DIAMOND_J) )
		if ( HandFaceLeft(FaceLeaded())>0 )
			return HandMaxCard(FaceLeaded());
	if ( IsCardOnGame(SPADE_Q) )
		if ( HandFaceLeft(FaceLeaded())>0 )
			return HandMinCard(FaceLeaded());
	switch(CurrentOrder())
	{
		//////////////////////////////////////////////////////////////
		case 1:
			if ( MyVar[MyPlayerID][0]==4 )
			{	///////// lead for chuan //////////
				if ( HandMoreCard(HEART_10)>0 )
					return HandNextCard(HEART_10);
			}
			if ( HandMoreCard(SPADE_J)==0 && HandLessCard(SPADE_Q)>0 )
				return HandMaxCard(SPADE);
			if ( HandLessCard(SPADE_Q)>2 && HandMoreCard(SPADE_Q)<2 )
				return HandPrevCard(SPADE_Q);
			if ( IsCardOnHand(DIAMOND_J) )
				if ( (IsCardOutGame(DIAMOND_Q)||IsCardOnHand(DIAMOND_Q)) &&
						(IsCardOutGame(DIAMOND_K)||IsCardOnHand(DIAMOND_K)) &&
						(IsCardOutGame(DIAMOND_A)||IsCardOnHand(DIAMOND_A)) )
					return DIAMOND_J;
			switch(MyVar[MyPlayerID][9]=HandMoreCard(DIAMOND_J))
			{
				case 3:
				case 2:
				case 1:
					MyVar[MyPlayerID][9] = 4-MyVar[MyPlayerID][9];
					if ( CountLeaded(DIAMOND)<MyVar[MyPlayerID][9] )
					{
						if ( HandLessCard(DIAMOND_J)>0 )
							return HandPrevCard(DIAMOND_J);
					}
					else
						if ( !IsCardOutGame(DIAMOND_J) )
							return HandNextCard(DIAMOND_J);
					break;
			}
			if ( HandMoreCard(DIAMOND_10)==0 )
				if ( CountLeaded(DIAMOND)<2 && HandFaceLeft(DIAMOND)>0 )
					return HandMaxCard(DIAMOND);
			if ( MyVar[MyPlayerID][0]!=4 )
				if ( HandMoreCard(HEART_Q)==0 )
					if ( HandLessCard(HEART_J)>0 )
						return HandPrevCard(HEART_J);
			if ( HandFaceLeft(CLUB)>0 )
			{
				if ( !IsGotCard(MyPlayerID,SPADE_Q) )
				{
					if ( HandMoreCard(SPADE_J)==0 )
						return HandMaxCard(CLUB);
					if ( IsGotCard(MyPlayerID,DIAMOND_J) )
						return HandMaxCard(CLUB);
				}
				return HandMinCard(CLUB);
			}
			if ( !IsCardOnHand(DIAMOND_J) )
				if ( HandFaceLeft(DIAMOND)>0 )
					return HandMinCard(DIAMOND);
			break;
		//////////////////////////////////////////////////////////////
		case 2:
			if ( HandFaceLeft(FaceLeaded())<=0 ) break;
			switch(FaceLeaded())
			{
				case SPADE:
					if ( IsCardOnHand(SPADE_Q) )
					{
						if ( IsCardOnGame(SPADE_K) || IsCardOnGame(SPADE_A) )
							return SPADE_Q;
						if ( HandFaceLeft(SPADE)>1 )
						{
							if ( HandLessCard(SPADE_Q)>0 )
								return HandMinCard(SPADE);
							if ( HandMoreCard(SPADE_Q)>0 )
								return HandMaxCard(SPADE);
						}
						if ( CountLeaded(SPADE)>3 )
							if ( !IsCardOutGame(SPADE_K) || !IsCardOutGame(SPADE_A) )
								return SPADE_Q;
					}
					if ( GameMaxCard()==SPADE_A )
						if ( IsCardOnHand(SPADE_K) )
							return SPADE_K;
					if ( HandLessCard(SPADE_Q)>0 )
						return HandPrevCard(SPADE_Q);
					if ( HandMoreCard(SPADE_Q)>0 )
						return HandNextCard(SPADE_Q);
					break;
				case CLUB:
					if ( CountGotCard(MyPlayerID,HEART)==13 )
					{
						if ( IsCardOnHand(CLUB_10) )
							if ( GameMaxCard()<CLUB_10 )
								return CLUB_10;
						if ( HandMoreCard(CLUB_10)>0 )
							return HandNextCard(CLUB_10);
					}
					if ( IsGotCard(MyPlayerID,DIAMOND_J) )
					{
						if ( IsCardOnHand(CLUB_10) )
							if ( GameMaxCard()<CLUB_10 )
								return CLUB_10;
						if ( HandMoreCard(CLUB_10)>0 )
							return HandNextCard(CLUB_10);
					}
					if ( !IsCardOnHand(CLUB_10) )
					{
						if ( IsGotCard(MyPlayerID,HEART_A) ||
								IsGotCard(MyPlayerID,HEART_K) ||
								IsGotCard(MyPlayerID,HEART_Q) ||
								IsGotCard(MyPlayerID,HEART_J) )
							return HandMinCard(CLUB);
						if ( CountGotCard(MyPlayerID,HEART)>4 )
							return HandMinCard(CLUB);
						if ( HandMoreCard(SPADE_J)>0 )
							return HandMinCard(CLUB);
					}
					if ( HandMoreCard(SPADE_J)>0 )
						if ( !IsCardOutGame(SPADE_Q) )
							return HandMinCard(CLUB);
					if ( HandMoreCard(HEART_10)==0 )
						return HandMaxCard(CLUB);
					if ( IsCardOnHand(CLUB_10) )
						if ( HandLessCard(CLUB_10)>HandMoreCard(CLUB_10) )
						{
							if ( HandLessCard(CLUB_10)>0 )
								return HandPrevCard(CLUB_10);
						}
						else
							if ( HandMoreCard(CLUB_10)>0 )
								return HandNextCard(CLUB_10);
					break;
				case HEART:
					if ( MyVar[MyPlayerID][0]==4 )
						if ( HandMoreCard(HEART_10)>0 )
							return HandNextCard(HEART_10);
					if ( HandLessCard(HEART_J)>0 )
						return HandPrevCard(HEART_J);
					else
						return HandMinCard(HEART);
				case DIAMOND:
					if ( IsCardOnHand(DIAMOND_J) )
					{
						if ( (IsCardOutGame(DIAMOND_Q)||IsCardOnHand(DIAMOND_Q)) &&
								(IsCardOutGame(DIAMOND_K)||IsCardOnHand(DIAMOND_K)) &&
								(IsCardOutGame(DIAMOND_A)||IsCardOnHand(DIAMOND_A)) &&
								GameMaxCard()<DIAMOND_J )
							return DIAMOND_J;
					}
					switch(HandMoreCard(DIAMOND_J))
					{
						case 3:
							if ( !IsCardOutGame(DIAMOND_J) )
								return HandNextCard(DIAMOND);
							break;
						case 2:
							if ( CountLeaded(DIAMOND)<1 )
							{
								if ( HandLessCard(DIAMOND_J)>0 )
									return HandPrevCard(DIAMOND_J);
							}
							else
								if ( !IsCardOutGame(DIAMOND_J) )
									if ( !IsCardOnGame(DIAMOND_Q) )
										if ( !IsCardOnGame(DIAMOND_K) )
											if ( !IsCardOnGame(DIAMOND_A) )
												return HandNextCard(DIAMOND_J);
							break;
						case 1:
							if ( CountLeaded(DIAMOND)<3 )
								if ( HandLessCard(DIAMOND_J)>0 )
									return HandPrevCard(DIAMOND_J);
							if ( CountLeaded(DIAMOND)>1 )
								if ( !IsCardOutGame(DIAMOND_J) )
									if ( !IsCardOnGame(DIAMOND_Q) )
										if ( !IsCardOnGame(DIAMOND_K) )
											if ( !IsCardOnGame(DIAMOND_A) )
												return HandNextCard(DIAMOND_J);
							if ( CountLeaded(DIAMOND)>2 )
								if ( !IsCardOutGame(DIAMOND_J) )
									if ( GameMaxCard()<DIAMOND_J )
										return HandMaxCard(DIAMOND);
							break;
						case 0:
							if ( CountLeaded(DIAMOND)<2 )
								return HandMaxCard(DIAMOND);
							else
							{
								if ( IsCardOnHand(DIAMOND_J) )
									if ( HandLessCard(DIAMOND_J)>0 )
										return HandPrevCard(DIAMOND_J);
							}
							break;
					}
					break;
			}
			break;
		//////////////////////////////////////////////////////////////
		case 3:
			if ( HandFaceLeft(FaceLeaded())<=0 ) break;
			switch(FaceLeaded())
			{
				case SPADE:
					if ( IsCardOnHand(SPADE_Q) )
					{
						if ( IsCardOnGame(SPADE_K) || IsCardOnGame(SPADE_A) )
							return SPADE_Q;
						if ( HandFaceLeft(SPADE)>1 )
						{
							if ( HandLessCard(SPADE_Q)>0 )
								return HandMinCard(SPADE);
							if ( HandMoreCard(SPADE_Q)>0 )
								return HandMaxCard(SPADE);
						}
						if ( CountLeaded(SPADE)>3 )
							if ( !IsCardOutGame(SPADE_K) || !IsCardOutGame(SPADE_A) )
								return SPADE_Q;
					}
					if ( !IsCardOnGame(SPADE_Q) )
						if ( !IsCardOnHand(SPADE_Q) )
							if ( IsVoid(NextPlayer(1),SPADE) )
								return HandMaxCard(SPADE);
					if ( IsCardOnGame(SPADE_A) )
						if ( IsCardOnHand(SPADE_K) )
							return SPADE_K;
					if ( IsVoid(NextPlayer(1),SPADE) )
						if ( HandMoreCard(SPADE_Q)>0 )
							return HandNextCard(SPADE_Q);
					if ( CountLeaded(SPADE)<2 )
						if ( HandLessCard(SPADE_Q)<2 && HandMoreCard(SPADE_Q)>0 )
							return HandNextCard(SPADE_Q);
					if ( HandLessCard(SPADE_Q)>0 )
						return HandPrevCard(SPADE_Q);
					if ( HandMoreCard(SPADE_Q)>0 )
						return HandNextCard(SPADE_Q);
					break;
				case CLUB:
					if ( CountGotCard(MyPlayerID,HEART)==13 )
					{
						if ( IsCardOnHand(CLUB_10) )
							if ( GameMaxCard()<CLUB_10 )
								return CLUB_10;
						if ( HandMoreCard(CLUB_10)>0 )
							return HandNextCard(CLUB_10);
					}
					if ( IsGotCard(MyPlayerID,DIAMOND_J) )
					{
						if ( IsCardOnHand(CLUB_10) )
							if ( GameMaxCard()<CLUB_10 )
								return CLUB_10;
						if ( HandMoreCard(CLUB_10)>0 )
							return HandNextCard(CLUB_10);
					}
					if ( !IsCardOnHand(CLUB_10) )
					{
						if ( IsGotCard(MyPlayerID,HEART_A) ||
								IsGotCard(MyPlayerID,HEART_K) ||
								IsGotCard(MyPlayerID,HEART_Q) ||
								IsGotCard(MyPlayerID,HEART_J) )
							return HandMinCard(CLUB);
						if ( CountGotCard(MyPlayerID,HEART)>4 )
							return HandMinCard(CLUB);
						if ( HandMoreCard(SPADE_J)>0 )
							return HandMinCard(CLUB);
					}
					if ( HandMoreCard(SPADE_J)>0 )
						if ( !IsCardOutGame(SPADE_Q) )
							return HandMinCard(CLUB);
					if ( HandMoreCard(HEART_10)==0 )
						return HandMaxCard(CLUB);
					if ( IsCardOnHand(CLUB_10) )
						if ( HandLessCard(CLUB_10)>HandMoreCard(CLUB_10) )
						{
							if ( HandLessCard(CLUB_10)>0 )
								return HandPrevCard(CLUB_10);
						}
						else
							if ( HandMoreCard(CLUB_10)>0 )
								return HandNextCard(CLUB_10);
					break;
				case HEART:
					if ( MyVar[MyPlayerID][0]==4 )
						if ( HandMoreCard(HEART_10)>0 )
							return HandNextCard(HEART_10);
					if ( HandLessCard(HEART_J)>0 )
						return HandPrevCard(HEART_J);
					else
						return HandMinCard(HEART);
				case DIAMOND:
					if ( IsCardOnHand(DIAMOND_J) )
					{
						if ( GameMaxCard()<DIAMOND_J )
							if ( IsVoid(NextPlayer(1),DIAMOND) )
								return DIAMOND_J;
						if ( (IsCardOutGame(DIAMOND_Q)||IsCardOnHand(DIAMOND_Q)) &&
								(IsCardOutGame(DIAMOND_K)||IsCardOnHand(DIAMOND_K)) &&
								(IsCardOutGame(DIAMOND_A)||IsCardOnHand(DIAMOND_A)) &&
								GameMaxCard()<DIAMOND_J )
							return DIAMOND_J;
//						if ( GameMaxCard()<DIAMOND_J )
//							if ( CountLeaded(DIAMOND)<2 )
//								if ( HandLessCard(DIAMOND_J)<2 )
//									if ( HandMoreCard(DIAMOND_J)==0 )
//										return DIAMOND_J;
					}
					switch(HandMoreCard(DIAMOND_J))
					{
						case 3:
							if ( !IsCardOutGame(DIAMOND_J) )
								return HandNextCard(DIAMOND);
							break;
						case 2:
							if ( CountLeaded(DIAMOND)<1 )
							{
								if ( HandLessCard(DIAMOND_J)>0 )
									return HandPrevCard(DIAMOND_J);
							}
							else
								if ( !IsCardOutGame(DIAMOND_J) )
									if ( !IsCardOnGame(DIAMOND_Q) )
										if ( !IsCardOnGame(DIAMOND_K) )
											if ( !IsCardOnGame(DIAMOND_A) )
												return HandNextCard(DIAMOND_J);
							break;
						case 1:
							if ( CountLeaded(DIAMOND)<3 )
								if ( HandLessCard(DIAMOND_J)>0 )
									return HandPrevCard(DIAMOND_J);
							if ( CountLeaded(DIAMOND)>1 )
								if ( !IsCardOutGame(DIAMOND_J) )
									if ( !IsCardOnGame(DIAMOND_Q) )
										if ( !IsCardOnGame(DIAMOND_K) )
											if ( !IsCardOnGame(DIAMOND_A) )
												return HandNextCard(DIAMOND_J);
							if ( CountLeaded(DIAMOND)>2 )
								if ( !IsCardOutGame(DIAMOND_J) )
									if ( GameMaxCard()<DIAMOND_J )
										return HandMaxCard(DIAMOND);
							break;
						case 0:
							if ( CountLeaded(DIAMOND)<2 )
								return HandMaxCard(DIAMOND);
							else
							{
								if ( IsCardOnHand(DIAMOND_J) )
									if ( HandLessCard(DIAMOND_J)>0 )
										return HandPrevCard(DIAMOND_J);
							}
							break;
					}
					break;
			}
			break;
		//////////////////////////////////////////////////////////////
		case 4:
			if ( HandFaceLeft(FaceLeaded())<=0 ) break;
			switch(FaceLeaded())
			{
				case SPADE:
					if ( IsCardOnHand(SPADE_Q) )
					{
						if ( IsCardOnGame(SPADE_K) || IsCardOnGame(SPADE_A) )
							return SPADE_Q;
						if ( HandFaceLeft(SPADE)>1 )
						{
							if ( HandLessCard(SPADE_Q)>0 )
								return HandMinCard(SPADE);
							if ( HandMoreCard(SPADE_Q)>0 )
								return HandMaxCard(SPADE);
						}
					}
					if ( MyVar[MyPlayerID][0]!=4 )
						return HandMaxCard(SPADE);
					else
						if ( !IsCardOutGame(SPADE_Q) )
							if ( HandMoreCard(SPADE_Q) )
								return HandNextCard(SPADE_Q);
					if ( HandLessCard(SPADE_Q)>0 )
						return HandPrevCard(SPADE_Q);
					if ( HandMoreCard(SPADE_Q)>0 )
						return HandNextCard(SPADE_Q);
					break;
				case CLUB:
					if ( CountGotCard(MyPlayerID,HEART)==13 )
					{
						if ( IsCardOnHand(CLUB_10) )
							if ( GameMaxCard()<CLUB_10 )
								return CLUB_10;
						if ( IsCardOnGame(CLUB_10) )
							return HandMaxCard(CLUB);
					}
					if ( IsGotCard(MyPlayerID,DIAMOND_J) )
					{
						if ( IsCardOnHand(CLUB_10) )
							if ( GameMaxCard()<CLUB_10 )
								return CLUB_10;
						if ( !IsCardOnGame(CLUB_10) )
							return HandMinCard(CLUB);
					}
					if ( !IsCardOnHand(CLUB_10) && !IsCardOnGame(CLUB_10) )
					{
						if ( IsGotCard(MyPlayerID,HEART_A) ||
								IsGotCard(MyPlayerID,HEART_K) ||
								IsGotCard(MyPlayerID,HEART_Q) ||
								IsGotCard(MyPlayerID,HEART_J) )
							return HandMaxCard(CLUB);
						if ( CountGotCard(MyPlayerID,HEART)>4 )
							return HandMaxCard(CLUB);
						if ( HandMoreCard(SPADE_J)>0 )
							return HandMaxCard(CLUB);
					}
					if ( HandMoreCard(SPADE_J)>0 )
						if ( !IsCardOutGame(SPADE_Q) )
							if ( !IsCardOnGame(CLUB_10) )
								return HandMaxCard(CLUB);
					if ( HandMoreCard(HEART_10)==0 )
						return HandMaxCard(CLUB);
					if ( IsCardOnHand(CLUB_10) )
						if ( HandLessCard(CLUB_10)>HandMoreCard(CLUB_10) )
						{
							if ( HandLessCard(CLUB_10)>0 )
								return HandPrevCard(CLUB_10);
						}
						else
							if ( HandMoreCard(CLUB_10)>0 )
								return HandNextCard(CLUB_10);
					break;
				case HEART:
					if ( MyVar[MyPlayerID][0]==4 )
						if ( HandMoreCard(HEART_10)>0 )
							return HandNextCard(HEART_10);
					if ( HandFaceLeft(HEART)>0 )
					{
						if ( HandLessCard(HEART_J)>0 )
							return HandPrevCard(HEART_J);
						else
							return HandMinCard(HEART);
					}
					break;
				case DIAMOND:
					if ( IsCardOnHand(DIAMOND_J) )
						if ( GameMaxCard()<DIAMOND_J )
							return DIAMOND_J;
					if ( HandLessCard(DIAMOND_J)>0 )
						return HandPrevCard(DIAMOND_J);
					if ( HandMoreCard(DIAMOND_J)>0 )
						return HandNextCard(DIAMOND_J);
					break;
			}
			break;
	}

	if ( CurrentOrder()>1 )
	{
		if ( HandFaceLeft(FaceLeaded())>0 )
			return RandomCard(FaceLeaded());
		else
		{
			///////////////////////// VOID ONLY //////////////////////////
			if ( IsCardOnHand(SPADE_Q) )
				return SPADE_Q;
			if ( MyVar[MyPlayerID][0]!=4 )
				if ( !IsCardOutGame(SPADE_Q) )
				{
					if ( IsCardOnHand(SPADE_A) ) return SPADE_A;
					if ( IsCardOnHand(SPADE_K) ) return SPADE_K;
				}
			if ( HandMoreCard(DIAMOND_10)==0 && HandLessCard(DIAMOND_J)>0 )
				return HandMaxCard(DIAMOND);
			if ( IsCardOnHand(CLUB_10) )
			{
				if ( IsGotCard(MyPlayerID,SPADE_Q) )
					return CLUB_10;
				if ( !IsGotCard(MyPlayerID,DIAMOND_J) )
					if ( !IsCardOnHand(DIAMOND_J) )
						return CLUB_10;
			}
			if ( HandFaceLeft(CLUB)>0 )
			{
				if ( IsGotCard(MyPlayerID,SPADE_Q) )
					return HandMaxCard(CLUB);
				if ( IsGotCard(MyPlayerID,DIAMOND_J) )
				{
					if ( HandLessCard(CLUB_10)>0 ) return HandPrevCard(CLUB_10);
					if ( HandMoreCard(CLUB_10)>0 ) return HandNextCard(CLUB_10);
					return CLUB_10;
				}
			}
			if ( IsCardOnHand(DIAMOND_J) )
			{
				if ( HandLessCard(DIAMOND_J)>0 ) return HandPrevCard(DIAMOND_J);
				if ( HandMoreCard(DIAMOND_J)>0 ) return HandNextCard(DIAMOND_J);
			}
			return RandomAnyCard();
		}
	}
	else
	{
		///////////////// lead any thing /////////////////////////
		if ( HandFaceLeft(CLUB)>0 ) return HandMinCard(CLUB);
		if ( HandFaceLeft(HEART)>0 ) return HandMinCard(HEART);
		if ( HandFaceLeft(SPADE)>0 ) return HandMinCard(SPADE);
		if ( HandLessCard(DIAMOND_J)>0 ) return HandPrevCard(DIAMOND_J);
		if ( HandMoreCard(DIAMOND_J)>0 ) return HandNextCard(DIAMOND_J);
		return RandomAnyCard();
	}
}
