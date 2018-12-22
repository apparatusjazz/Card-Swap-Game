#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

//My Ship Sails - card game

class Card{
  public:
    Card(){}
    Card(string a, int b); 

    void set_type(string a);
    void set_index(int a);
    void print_card() const;
    int get_index() const;
    string get_type() const;

  private:
    string cardType;  //suit/index like H2 for 2 of hearts
    int cardIndex;    //Each suit has a number index
};

Card::Card(string a, int b){
  set_type(a);
  set_index(b);
}
void Card::set_type(string a){
  cardType = a;
}
void Card::set_index(int a){
  cardIndex = a;
}
void Card::print_card() const {
  cout << cardType << ", " << cardIndex;
}
int Card::get_index() const {
  return cardIndex;
}
string Card::get_type() const {
  return cardType;
}

////////////////////////////////////////////////////////////////////
class Player{
  public:
    Player(){}
    virtual void chooseDiscard(char){}; //choose card to discard
    void replenish();                   //discarded card into hand
    int get_selfIndex() const;
    void print_hand() const;
    bool is_hand_complete() const;
    Card get_discard()const;

  protected:
    int selfIndex;      //index within player vector, 0 for human
    int takeFrom;       //Index of player supplying card
    vector<Card> hand;  //hand that holds 7 cards
    Card discard;       //Card being discarded
    int discardIndex;   //position within hand of card being discarded
    static Card discardList[5]; //All discarded cards from players go into this array

};

Card Player::discardList[5] = {};

void Player::replenish(){
  hand[discardIndex] = discardList[takeFrom];
}
int Player::get_selfIndex() const{
  return selfIndex;
}
void Player::print_hand() const{
  cout << get_selfIndex() << ":  ";
  for(int i = 0; i < 7; i++){
    cout << setw(5) << hand[i].get_type();
  }
}
bool Player::is_hand_complete() const{
  int a = 0;
  for(int i = 1; i < 7; i ++){
    if(hand[i].get_index() == hand[i-1].get_index()){
      a++;
    }
  }
  if(a == 6){return true;}
  else{return false;}
}
Card Player::get_discard()const {
  return discard;
}



///////////////////////////////////////////////////////////////////
class Human : public Player {
  public:
    Human(int selfI, int take, vector<Card> h);
    void chooseDiscard(char cardLetter);
    bool get_isCharValid() const;   //true if character imput is from a - g
  private:
    bool isCharValid;
};

Human::Human(int selfI, int take, vector<Card> h){
  selfIndex = selfI;
  discardIndex = 0;
  takeFrom = take;
  hand = h;
  discard = Card("H1", 1);
}

void Human::chooseDiscard(char cardLetter){
  isCharValid = true;
  switch(cardLetter)
  {
    case 'a': discardIndex = 0; break;
    case 'b': discardIndex = 1; break;
    case 'c': discardIndex = 2; break;
    case 'd': discardIndex = 3; break;
    case 'e': discardIndex = 4; break;
    case 'f': discardIndex = 5; break;
    case 'g': discardIndex = 6; break;
    default: isCharValid = false; break; //used when input invalid in main
  }
  discard = hand[discardIndex];
  discardList[selfIndex] = discard;
}

bool Human::get_isCharValid() const{
  return isCharValid;
}


///////////////////////////////////////////////////////////////////
class AutoPlayer : public Player {
  public:
    AutoPlayer(int selfI, vector<Card> h);
    void set_takeFrom();
    void chooseDiscard(char);

};

AutoPlayer::AutoPlayer(int selfI, vector<Card> h){
  selfIndex = selfI;
  hand = h;
  set_takeFrom();
  discard = Card("H1", 1);
}
void AutoPlayer::set_takeFrom(){
  takeFrom = selfIndex - 1;
}

void AutoPlayer::chooseDiscard(char){
  int counter = 0, indexTracker = 0, temp1 = 0;
  vector<int> cardIndexes(7, 0);

  for(int i = 0; i < 7; i++){ //Putting the indexes from hand into a vector
    cardIndexes[i] = hand[i].get_index();
  }
  sort(cardIndexes.begin(), cardIndexes.end()); //Sorting the vector in order

  for(int i = 1; i < 7; i ++){//This finds the most populous index, or the most populous suit
    if(cardIndexes[i] == cardIndexes[i - 1]){
      counter ++;
      if(counter > temp1){
        temp1 = counter;
        indexTracker = cardIndexes[i];
      }
    }
    else{
      counter = 0;
    }
  }
  //Now we have the most populous index number stored in indexTracker. The player will now pick a card that is not the indexTracker number.
  for(int i = 0; i < 7; i ++){
    if(hand[i].get_index() != indexTracker){
      discard = hand[i];
      discardIndex = i;
      break;
    }
  }
  discardList[selfIndex] = discard;

}
///////////////////////////////////////////////////////////////


void print_hands(vector<Player*> p);
void replenish_hands(vector<Player*> p);
void players_discard(vector<Player*> p);

int main() {

  srand(time(0));
  char charInput;
  bool winnerExists = false;

  vector<string> suits{"H", "S", "D", "C"};
  vector<string> rank{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  vector<string> deckset;

  Card setter("H1", 1);
  vector<Card> deck(52, setter); 

  for(int i = 0, g = 1, c = 0; i < 4; i++, g++){
    for(int j = 0; j < 13; j++){
      deckset.push_back(suits[i] + rank[j]);
      deck[c].set_type(deckset[c]);
      deck[c].set_index(g);
      c++;
    }
  }

  random_shuffle(deck.begin(), deck.end());

  vector<Card> genericHand(7, setter);
  vector<vector <Card> > handsx5(5, genericHand);

  int a = 0;
  for(int i = 0; i < 5; i ++){
    for(int j = 0; j < 7; j ++){
      handsx5[i][j] = deck[a];
      a ++;
    }
  }

  Human player0(0, 4, handsx5[0]);
  AutoPlayer player1(1, handsx5[1]);
  AutoPlayer player2(2, handsx5[2]);
  AutoPlayer player3(3, handsx5[3]);
  AutoPlayer player4(4, handsx5[4]);

  vector<Player*> players {&player0, &player1, &player2, &player3, &player4}; //used to access hands of all players

  vector<char> a_to_g {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

  while(1){

    cout << "Your hand:" << endl;
    player0.print_hand();
    cout << endl << "    ";

    for(int i = 0; i < 7; i ++){
      cout << setw(5) << a_to_g[i];
    }
    cout << endl;

    while(1){
      cout << "cmd: ";
      cin >> charInput;

      if(charInput == '?'){
        print_hands(players);
        continue;
      }
      players[0]->chooseDiscard(charInput); //Choose card to discard for human, the while loop tests correct character input

      if(player0.get_isCharValid()){break;}
      else cout << "Invalid choice" << endl;
    }

    players_discard(players);
    replenish_hands(players);

    for(int i = 0; i < 4; i++){
      cout << "       Player " << i << " gave " << i + 1 << " ";
      cout << players[i]->get_discard().get_type() << endl;;
    }
    cout << "       Player 4 gave 0 " << players[4]->get_discard().get_type() << endl;


    for(int i = 0; i < 5; i ++){
      if(players[i]->is_hand_complete()){
        cout << "Player " << i << " is a winner!" << endl;
        winnerExists = true;
      }
    }
    if(winnerExists){
      break;
    }
  }
  cout << "At the end of the game the players had:" << endl;
  print_hands(players);
  return 0;    
}

void print_hands(vector<Player*> p){
  for(int i = 0; i < p.size(); i ++){
    p[i]->print_hand();
    cout << endl;
  }
}
void replenish_hands(vector<Player*> p){
  for(int i = 0; i < 5; i++){
    p[i]->replenish();
  }
}
void players_discard(vector<Player*> p){
  for(int i = 1; i < 5; i++){
    p[i]->chooseDiscard('a');
  }
}
