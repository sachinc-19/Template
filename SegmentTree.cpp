#include<bits/stdc++.h>
#pragma GCC target ("sse4.2")
using namespace std;
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//using namespace __gnu_pbds;
#define int long long int
#define ld long double
#define fon(i,n) for(int i=0;i<n;i++)
#define fo(i,n) for(int i=1;i<=n;i++)
#define pb push_back
#define mp make_pair
#define eb emplace_back
#define ff first
#define ss second
#define pii pair<int,int>
#define vii vector<int>
#define bs binary_search
#define lb lower_bound
#define ub upper_bound
#define setbits(x) __builtin_popcountll(x)
#define M (1000*1000*1000+7)
#define sz(x) ((int)(x).size())
#define endl "\n"
#define test int T; cin>>T; while(T--)
#define all(z) z.begin() , z.end()
#define allr(z) z.rbegin() , z.rend()
#define sp(x,y)   fixed << setprecision(x) << y
#define memo(oo , zz) memset(oo , zz , sizeof(oo))
//template<typename T>
//using ordered_set = tree<T , null_type,less<T>, rb_tree_tag,tree_order_statistics_node_update>;

// DEBUG START=======================================================================================
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}

template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif
// DEBUG END =====================================================================================

bool sortbyme(pii a , pii b)
{
    return a.ss > b.ss;
}

const int maxn = 1e5;
const int INF = 1e9;
/*
KEYNOTES:
----------------------------------------------
merge(X , identity_element) = X
----------------------------------------------
identity_transformation.combine(X) = X
----------------------------------------------
----------------------------------------------
ALWAYS: older_update.combine(newer_update)
----------------------------------------------
*/

// example: addition: identity element is 0
// a + 0 = 0 +

struct node
{
    int v = 0; // 1
    // use more variables if you want more information
    // these default values should be identity element
    node(){}
    node(int val){  // we are talking about the leaf
        v = val; // 2
    }
    void merge(const node &l , const node &r){ // store the thing you wanna query
       v = l.v + r.v;
    //    v = l.v + r.v;  // 3
       // If we wanted the maximum then we would do
       // like v = max(l.v , r.v)
    }
};

// add on a range :identity transformation = 0
// old += new

// If old is identity which is 0 , then 0 += new = new

struct update
{
    int v = 0; //4
    // use more variables if you want more information
    // these default values should be identity transformation
    update(){}
    update(int val){
        v = val; //5
    }
    // combine the current update with the other update
    void combine(update &other , const int32_t &tl , const int32_t &tr)
    {
        v = other.v;  //6
        // you can be sure that the other is newer than current
    }
    // store the correct infromation in the node X
    void apply(node &x , const int32_t &tl , const int32_t &tr)
    {
        x.v = (tr-tl+1)*v;  //7
        // no change in freq
    }
};

// template<typename node , typename update>
class segtree
{
   private:
   int len;
   vector<node> t;
   vector<update> u;
   vector<bool> lazy;
   node identity_element;
   update identity_transformation;
   void pushdown(const int &v , const int &tl , const int &tr)
   {
       if(lazy[v]) // if it is lazy , propagate it
       {
           lazy[v] = 0;  // not lazy anymore push the update down
           // he was remembering upd[v]
   
           int mid = (tl+tr)/2;
           apply(2*v , tl , mid , u[v]);  // propagate on left child
           apply(2*v+1 , mid+1 , tr , u[v]);  // propagate on right child
           u[v] = identity_transformation; //update done
           // man doesn't remember anything anymore
       }
   }
   void apply(const int32_t &v , const int32_t &tl , const int32_t &tr , update upd) // tl , tr is the node range and v is the node id
   {
       if(tl != tr) // if tl is not a leaf mark it lazy
       {
           lazy[v] = 1;
           u[v].combine(upd , tl , tr);
       }
       upd.apply(t[v] , tl , tr);
   }

   template<typename T>
   void build(const T &arr , const int &v , const int &tl , const int &tr){
       if(tl == tr)
       {
           t[v] = arr[tl];
           return;
       }
       int tm = (tl+tr) >> 1;
       build(arr , v<<1 , tl , tm);
       build(arr , v<<1|1 , tm+1 , tr);
       t[v].merge(t[v<<1] , t[v<<1|1]);
   }
   // pretty much unchanged except for the pushdown
   node query(const int &v , const int &tl , const int &tr , const int &l , const int &r)
   {
       if(tl > r || tr < l){
            return identity_element;
       }
       if(tl >= l && tr <= r){
           return t[v];
       }
       pushdown(v , tl , tr); // we remove lazy tag before going down
       // why? So that the children have correct information
       // previously children have wrong info
   
       int tm = (tl+tr)/2;
       node a = query(2*v , tl , tm , l , r) , b = query(2*v+1 , tm+1 , tr , l , r) , ans;
       ans.merge(a , b);
       return ans;
   }
   // l , r is the update range
   // val is the update value , add this to all the leaaves in the range [l , r]
   // rupd = range update
   void rupd(const int &v , const int &tl ,const int &tr , const int &l ,const int &r ,const update &upd)
   {
       if(tl > r || tr < l){ // no overlap
           return ;
       }
       if(tl >= l && tr <= r){ // fully within
           // ask the man to remember it
           apply(v , tl , tr , upd);
           return;
       }
       // partial overlap
       pushdown(v , tl , tr);// we remove lazy tag before going down
       // why? So that the children have correct information
       // previously children have wrong info
   
       int tm = (tl+tr)/2;
       rupd(2*v , tl , tm , l , r , upd);
       rupd(2*v+1 , tm+1  , tr , l , r , upd);
       t[v].merge(t[2*v] , t[2*v+1]); // store correct value here at this node
   }

   public:
   segtree(int l){
       len = l;
       t.resize(4 * len);
       u.resize(4 * len);
       lazy.resize(4 * len);
       identity_element = node();
       identity_transformation = update();
   };
   template<typename T>
   void build(const T &arr)
   {
       build(arr , 1 , 0 , len-1);
   }
   node query(const int &l , const int &r){
       return query(1 , 0 , len-1 , l , r);
   }
   void rupd(const int &l , const int &r , const update &upd){
       rupd(1 , 0 , len-1 , l , r , upd);
   }
};

signed main()
{
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);

   segtree s(1000);  // create a segment tree of length  1000
   vector<int> v(1000);
   s.build(v);
   s.rupd(2 , 5 , 8); // l , r , val
   s.rupd(1 , 3 , -4);
   // printing values from index 0 to 15
   for(int i = 0 ; i < 15 ; i++)
   {
       cout << s.query(i , i).v << " ";
   }
   cout << endl;
   cout << s.query(4 , 5).v << " " << s.query(2 , 4).v << endl;
   return 0;
}

// how to define the behaviour or the nature of the update and queries

// this segment was sum in range and add on range

// let's change it to -> (sum on range) and (set all values on range to certain value)

// let's make one from the question earlier

// get min element and frequency of the min element in the range
