#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Set;
class City;

vector<City*> CityMap(1000000, nullptr); // 특정 번호를 가진 도시 클래스를 담는 벡터
int leaderNumber = 0; // Union이 발생할 때, 새로운 집합의 Leader 번호를 저장하는 변수

// 도시에 대한 정보를 담고 있는 클래스
// 도시 번호, 이름, 인구수, 속해있는 Set, 다음에 연결돼 있는 도시 변수가 있다.
// Single Linked List로 구현돼 있다.
class City {
public:
    int number;
    string name;
    int population;
    Set* set;
    City* next;
    // 본인이 속해있는 집합을 가리키는 포인터가 있습니다.
    // Set 내부에는 Single Linked-List로 도시가 연결되어 있습니다.

    City(int number, string name, int population) {
        this->number = number;
        this->name = name;
        this->population = population;
        this->set = nullptr;
        this->next = nullptr;
    }
    ~City() {}
    // 메모리 관리는 개발자의 숙명이기 때문에, 소멸자를 꼭 생성합니다.
};

// Set을 구성하는 클래스
// setNumber는 Set의 leader 도시의 번호를 저장하고, Head와 Tail에 각각 가장 앞에 있는 도시와 가장 뒤에 있는 도시를 저장한다.
// Size는 Set에 있는 도시 수를 나타내며, roadDistance는 Set을 이루는 도시의 모든 포장도로의 합이다.
class Set {
public:
    int setNumber;
    City* head;
    City* tail;
    int size;
    int roadDistance;
    // head와 tail 포인터를 가지고 있고, leader Number와 크기, 포장도로의 총 합을 저장하는 변수가 있습니다.

    Set() {
        this->setNumber = -1;
        this->head = nullptr;
        this->tail = nullptr;
        this->size = 0;
        this->roadDistance = 0;
    }
    ~Set() {}
    // 소멸자를 생성합니다.
};


// UnionFind 클래스
// 각 Set은 벡터에 저장하였다.
class UnionFind {
public:
    vector<Set*> setOfCity;

    UnionFind() {}
    void Union(Set* a, Set* b);
    Set* findSet(int number);
    City* findCity(int number);
    void AddNewSet(int number, string name, int population);
    void Combine(Set* a, Set* b);
};

void UnionFind::Union(Set* a, Set* b) { //Union 함수
    if (a->size > b->size) { // a Set이 사이즈가 더 클 경우
        Combine(a, b);
        leaderNumber = a->setNumber;
    }
    else if (a->size < b->size) { // b Set이 사이즈가 더 클 경우
        Combine(b, a);
        leaderNumber = b->setNumber;
    }
    else { // 두 Set이 사이즈가 같을 때,
        if (a->head->number < b->head->number) { // 리더의 도시 번호로 판별한다.
            Combine(a, b);
            leaderNumber = a->setNumber;
        }
        else {
            Combine(b, a);
            leaderNumber = b->setNumber;
        }
        // leaderNumber 변수는 최근 Union이 이루어진 Set의 leader의 도시 번호를 뜻한다. 뒤에 나오는 함수들에서 사용하기 위하여 전역변수로 선언했다.
        // Combie 함수는 아래 서술하겠지만, 두 Set을 합쳐주는 함수이다. 앞 파라미터의 Set에 뒷 파라미터의 Set을 붙여준다.
    }
}

Set* UnionFind::findSet(int number) { // Set을 찾아서 리턴해주는 함수이다.
    Set* retSet = CityMap[number]->set; // 특정 도시 번호로 접근하여, 그 도시가 가리키고 있는 Set을 리턴한다.

    return retSet;
}

City* UnionFind::findCity(int number) { // 도시를 찾아서 리턴해주는 함수이다.
    City* retCity = CityMap[number]; // 특정 도시 번호로 해당 벡터에 바로 접근하여, 리턴할 수 있다.

    return retCity;
}

void UnionFind::AddNewSet(int number, string name, int population) { // 새로운 Set을 만들어주는 함수
    Set* newSet = new Set();
    City* newCity = new City(number, name, population);
    // 새로 만들어지는 도시는 본인이 Set이므로 Set과 City를 함께 만들어준다.

    newSet->head = newCity;
    newSet->tail = newCity;
    newSet->size = 1;
    newSet->setNumber = newCity->number;

    // 각 Set의 head, tail, size, leader를 설정해준다.
    // 처음에는 도시 하나만 있으므로, head, tail, leader를 모두 해당 도시를 가리키게 한다.

    newCity->set = newSet;
    newCity->next = nullptr;
    // 새로 만들어지는 도시의 set변수를 해당 set으로 설정하고, next는 nullptr로 지정한다.

    CityMap[number] = newCity;
    // 도시 번호는 유일하므로, 해당 도시 번호를 인덱스로 매핑하여 저장한다.

    setOfCity.push_back(newSet);
    // 각 Set은 벡터에 저장한다. 벡터가 Union 역할을 한다.
}

void UnionFind::Combine(Set* a, Set* b) { // 두 Set을 결합하는 함수이다. b Set을 a Set에 붙인다.
    City* city = b->head;

    while (city != nullptr) {
        city->set = a;
        city = city->next;
    }
    // b Set에 있는 모든 도시가 가리키고 있는 Set을 a로 바꿔준다.

    a->tail->next = b->head;
    a->tail = b->tail;
    a->size += b->size;
    a->roadDistance += b->roadDistance;

    // a Set의 맨 뒤에 b를 연결해준다.
    // a Set의 tail에 b의 tail을 넣어준다.
    // 또한 W 함수를 위하여, Set의 모든 포장도로 합을 저장해야 한다. b Set이 가지고 있던 포장도로 합을 a Set 포장도로 합에다가 더해준다.

    delete b;
    // delete를 하여 동적할당 된 Set을 메모리에서 해제한다.
}

// Edge 클래스
class Edge {
public:
    int sCity;
    int dCity;
    int distance;
    int pop;

    Edge(int sCity, int dCity, int distance) {
        this->sCity = sCity;
        this->dCity = dCity;
        this->distance = distance;
        int Ingu = CityMap[sCity]->population + CityMap[dCity]->population; // 도시 번호를 통해 벡터로 접근하여, 도시 인구수를 Edge 변수에 저장해둔다.
        this->pop = Ingu;
    }
};
// Edge 또한 여러 변수를 가지고 있다. sCity, dCity는 각각 연결된 도시를 의미하고, distance는 거리, pop는 연결된 양쪽 도시의 인구수 합이다.


// 구조체와 같이 변수가 여러 개인 자료형의 정렬에선 comp 함수가 필요하다. (Algorithmn 함수의 sort를 사용할 때)
bool comp(const Edge& a, const Edge& b) {

    // 총 세 가지의 경우의 수가 있다.
    // 1) 거리, 2) 인구수, 3) 도시 번호 순으로 정렬 우선순위가 있다.

    int v1, v2;

    if (a.sCity < a.dCity)
        v1 = a.sCity;
    else
        v1 = a.dCity;

    if (b.sCity < b.dCity)
        v2 = b.sCity;
    else
        v2 = b.dCity;
    // 연결된 Edge의 낮은 도시 번호를 도출하는 과정이다.

    if (a.distance == b.distance) { // 거리가 같은 경우
        if (a.pop == b.pop) // 인구 수가 같은 경우
            return v1 < v2; // 도시 번호가 작은 edge가 보다 앞에 정렬된다.

        return a.pop > b.pop; // 인구수가 많은 edge가 보다 앞에 정렬된다.
    }
    return a.distance < b.distance; // 거리가 더 짧은 edge가 보다 앞에 정렬된다.
}

UnionFind uni; // Union-Find 객체이다.
vector<Edge> edgeList; // 입력 받을 때, 모든 edge를 저장하는 벡터 변수이다.
vector<Edge> compleEdgeList; // compleEdgeList는 MST에 포함되는 edge를 모아둔 벡터 변수이다.

int kruskalIndex = 0; // kruskal 알고리즘을 사용할 때, edgeList에 접근하기 위한 인덱스 변수이다. 벡터의 erase 함수를 사용하니, 시간 초과가 나는 경우가 생겨서 만들었다.

// N 질의를 입력받았을 때, 실행되는 함수이다.
void Nfunc(int v, int k) {
    Set* a = uni.findSet(v);
    // v 도시가 속해있는 Set을 찾는다.
    City* cur = a->head;
    // v 도시의 가장 앞에 있는 도시를 cur 변수에 저장한다.

    for (int i = 0; i < k; i++) {
        if (cur == nullptr) {
            cout << "no exist\n";
            return;
            // k 번째까지 갔는데 아무것도 없다면, "no exist"를 출력한다.
        }
        else {
            cur = cur->next;
            // nullptr이 아니라면, 다음 도시로 간다.
        }
        // k번째 도시를 찾는 게 목적이므로, 반복문을 k번 돌린다.
    }

    // k - 1까지 갔는데도, nullptr인 경우가 있다. k - 1번까지 반복문이 실행되고나서, cur에 cur->next가 저장되어 반복문에서 예외처리 못한 상태로 cur에 nullptr이 들어가는 경우다.
    if (cur == nullptr)
        cout << "no exist\n";
    else
       cout << cur->number << ' ' << cur->name << '\n';
    // cur가 nullptr이 아니라면, 도시의 번호와 이름을 출력한다.
}

// L 질의가 입력됐을 때, 실행되는 함수이다.
void Lfunc(int v) {
    Set* a = uni.findSet(v);
    // 찾고자 하는 도시가 속해있는 Set을 찾는다.

    cout << a->size << '\n';
    // 해당 Set의 도시 수를 출력하는 함수이므로, 해당 클래스에 저장되어있던 size 변수를 그대로 출력한다.
}

// I 질의가 입력됐을 때, 실행되는 함수이다.
void Ifunc(int N) {
    Edge start = edgeList[kruskalIndex];
    // edgeList는 입력받는 모든 간선이 저장되어있는 벡터이다. 이는 메인 함수에서 조건에 맞게, 가장 우선적으로 MST를 구성해야 하는 간선부터 정렬됐다.
    // kruskalIndex 변수는 정렬된 edgeList를 하나씩 증가하며, 해당 간선을 확인한다.

    Set* a = uni.findSet(start.sCity);
    Set* b = uni.findSet(start.dCity);
    // edgeList에서 가져온 간선에 연결된 두 도시의 Set을 찾는다.

    if (a->setNumber == b->setNumber) {
        cout << "not union\n";
        kruskalIndex++;
    }
    // 두 도시가 속해있는 Set이 같다면, 이는 Cycle을 형성한다는 뜻이므로, Union하지 않는다.
    // 다음 간선을 확인하기 위해 kruskalIndex를 1 증가시킨다.
    else {
        // Cycle을 형성하지 않는 경우이다.
        uni.Union(a, b);
        // 두 Set을 Union한다.
        compleEdgeList.push_back(edgeList[kruskalIndex]);
        // compleEdgeList는 MST를 구성하는 간선을 저장하는 벡터다.
        CityMap[leaderNumber]->set->roadDistance += edgeList[kruskalIndex].distance;
        // 새로 생성된 Set에 병합되는 Set이 갖고 있던 포장도로의 총합을 더해준다.
        kruskalIndex++;
        // 다음 간선을 확인하기 위해 kruskalIndex를 1 증가시킨다.

        if ((int)compleEdgeList.size() == N - 1) {
            // MST가 완성됐다는 것은, MST를 구성하는 간선의 수가 '정점 - 1'이 됐다는 의미와 같다.
            // 그러므로, compleEdgeList의 사이즈가 '정점 - 1'이라면, 조건에 맞게 결과값을 출력한다.
            
            cout << leaderNumber << ' ' << uni.findSet(leaderNumber)->size << ' ' << leaderNumber << ' ' << uni.findSet(leaderNumber)->roadDistance << '\n';
        }
        else {
            // MST가 완성되지 않았을 경우
            cout << leaderNumber << ' ' << uni.findSet(leaderNumber)->size << '\n';
        }
    }
}

// F 질의가 입력됐을 때, 실행하는 함수이다.
void Ffunc(int cityNumber1, int cityNumber2) {
    Set* a = uni.findSet(cityNumber1);
    Set* b = uni.findSet(cityNumber2);
    // 입력받은 도시들이 속한 Set을 찾는다.

    if (a == b) {
        // 두 Set이 같다면, 같은 Set에 속해있다는 의미다.
        cout << "True " << a->head->number << '\n';
    }
    else {
        // 다르다면, 둘은 다른 Set에 속해있다는 의미다.
        cout << "False " << a->head->number << ' ' << b->head->number << '\n';
    }
}

// W 질의가 입력됐을 때, 실행하는 함수이다.
void Wfunc(int v) {
    City* city = uni.findCity(v);
    // 입력받은 도시를 찾는다.

    cout << city->set->roadDistance << '\n';
    // 해당 도시가 속한 Set의 모든 포장도로의 합 변수가 있으므로, 출력한다.
}

// Q 질의가 입력됐을 때, 실행하는 함수이다.
void Qfunc(int N) {
    // 반복문을 MST가 완성될 때까지 실행시킨다.
    while (true) {
        if ((int)compleEdgeList.size() == N - 1)
            break;
        // 앞서 언급했듯이, MST의 간선이 '정점 - 1'이라면, MST가 완성됐다는 말이므로, 반복문을 끝낸다.

        Edge start = edgeList[kruskalIndex];

        Set* a = uni.findSet(start.sCity);
        Set* b = uni.findSet(start.dCity);

        if (a == b) {
            kruskalIndex++;
            continue;
        }
        // Cycle을 형성하는 경우에는 아무것도 하지 않고 넘어간다.

        uni.Union(a, b);

        compleEdgeList.push_back(edgeList[kruskalIndex]);
        CityMap[leaderNumber]->set->roadDistance += edgeList[kruskalIndex].distance;
        kruskalIndex++;
        // Cycle을 형성하지 않는 경우에는 Union을 실행시킨다.
        // MST가 완성될 때까지 반복문을 실행시키는 것을 제외하고, 모두 I 질의를 입력받았을 때와 같다.
    }

    int num = 0;

    for (int i = 0; i < (int)uni.setOfCity.size(); i++) {
        if (uni.setOfCity[i]->size == N) {
            num = uni.setOfCity[i]->setNumber;
            break;
        }
    }
    // 완성된 MST의 leader를 찾는 과정이다. 크기가 가장 큰 set이 MST이므로, 해당 set의 leader를 출력한다.

    cout << num << ' ' << uni.findSet(leaderNumber)->roadDistance << '\n';

    return;
}

int main(void) {

    int N, M, Q;

    cin >> N >> M >> Q;

    uni.setOfCity.reserve(50000);
    edgeList.reserve(100000);
    // 시간초과가 났던 가장 큰 이유였다. 벡터는 데이터가 많을 수록, push_back과 erase를 할수록 시간적 효율이 굉장히 떨어진다.
    // 그러므로, resize 또는 reserve를 해당 최댓값만큼 설정한다면, 이 문제에서 Doubling을 하는 경우는 없다.
    // 그래서 시간을 상당히 단축시킬 수 있다.

    for (int i = 0; i < N; i++) {
        int number, population;
        string name;
        cin >> number;
        cin >> name;
        cin >> population;
        uni.AddNewSet(number, name, population);
    }
    // 도시 정보를 입력받는 반복문이다.

    for (int i = 0; i < M; i++) {
        int sCity, dCity, distance;
        cin >> sCity >> dCity >> distance;
        Edge newEdge = Edge(sCity, dCity, distance);
        edgeList.push_back(newEdge);
    }
    // 간선 정보를 입력받는 반복문이다. 해당 간선을 edgeList 벡터에 삽입한다.

    sort(edgeList.begin(), edgeList.end(), comp);
    // algorithm 헤더의 sort 알고리즘으로 조건에 맞게 간선들을 정렬한다.

    // 질의를 입력받아, 종류에 따라 해당 함수를 실행시키는 반복문이다.
    for (int i = 0; i < Q; i++) {
        char Query;
        cin >> Query;

        if (Query == 'N') {
            int v, k;
            cin >> v >> k;
            Nfunc(v, k);
            
        }
        else if (Query == 'L') {
            int v;
            cin >> v;
            Lfunc(v);
        }
        else if (Query == 'I') {
            Ifunc(N);
        }
        else if (Query == 'F') {
            int v1, v2;
            cin >> v1 >> v2;
            Ffunc(v1, v2);
        }
        else if (Query == 'W') {
            int v;
            cin >> v;
            Wfunc(v);
        }
        else if (Query == 'Q') {
            Qfunc(N);
        }
    }
}
