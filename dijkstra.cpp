// dijkstra.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <queue>
#include <string>
#include <map>
#include <climits>
#include <fstream>
#include <stack>
using namespace std;
const int maxn = 2500, inf = INT_MAX >> 1;
struct dta
{
    int lc1, lc2;
    string str1, str2;  
};
bool cmp(dta a, dta b)
{
    return a.lc1 < b.lc1;
}
struct EDG
{
    int from, to, len;
    vector<dta> sta;
    bool operator <(const EDG& x)const
    {
        return len > x.len;
    }
};
map<string, int> loc;
int h[maxn], nxt[maxn << 1], esize = 0;
EDG edg[maxn << 1];
inline void add1(int n1, int n2, int len)
{
    esize++;
    edg[esize].from = n1; edg[esize].to = n2; edg[esize].len = len;
    nxt[esize] = h[n1];
    h[n1] = esize;
}
inline void add2(int n1, int lc1, int n2, int lc2, string str1, string str2)
{
    if (lc1 == 0 && n2 == 0 && lc2 == 0)//节点站，用一个特殊边存，指向自己，边长为0，用tag1记录特殊边
    {
        esize++;
        edg[esize].from = edg[esize].to = n1;; edg[esize].len = 0;
        nxt[esize] = h[n1];
        h[n1] = esize;
        edg[esize].sta.push_back({ lc1,lc2,str1,str2 });
        loc[str2] = esize;
    }
    else
    {
        bool flg = 1;
        for (int i = h[n1]; i; i = nxt[i])
        {
            if (edg[i].to == n2 || (n2 < 0 && edg[i].to < 0))
            {
                flg = 0;
                edg[i].sta.push_back({ lc1,lc2,str1,str2 });
                loc[str2] = i;
                break;
            }
        }
        if (flg)
        {
            esize++;
            edg[esize].from = n1; edg[esize].to = n2; edg[esize].len = inf;
            nxt[esize] = h[n1];
            h[n1] = esize;
            edg[esize].sta.push_back({ lc1,lc2,str1,str2 });
            loc[str2]=esize;
        }
    }
}
void dijkstra(string S, string E)
{
    cout << "发站:" << S << '\n';
    cout << "到站:" << E << '\n';
    int s = loc[S], e = loc[E];
    printf("出发边:%d %d\n", edg[s].from, edg[s].to);
    printf("终点边:%d %d\n", edg[e].from, edg[e].to);
    priority_queue<EDG> Q;
    vector<int> vis(maxn, 0), dis(maxn, inf), pre(maxn, 0);
    //将他节点站的边入队
//for (int i = h[edg[s].from]; i; i = nxt[i])
//{
//    if (edg[i].from == edg[i].to)continue;
//    if (edg[i].to < 0)continue;
//    Q.push({ edg[i].from,edg[i].to,edg[i].len });
//}
    if (edg[s].to < 0)
    {
        Q.push({ edg[s].from, edg[s].from, 0 });
    }
    else
    {
        Q.push({ edg[s].to, edg[s].to, 0 });
    }
    while (!Q.empty())
    {
        EDG now = Q.top(); Q.pop();
        vis[now.to] = 1;
        if (now.len > dis[now.to])continue;
        for (int i = h[now.to]; i; i = nxt[i])
        {
            if (edg[i].from == edg[i].to)continue;
            if (edg[i].to < 0)continue;
            if (!vis[edg[i].to] && dis[edg[i].to] > now.len + edg[i].len)
            {
                pre[edg[i].to] = now.to;
                dis[edg[i].to] = now.len + edg[i].len;
                Q.push({ edg[s].from,edg[i].to,dis[edg[i].to] });
            }
        }
    }
    int ans = dis[edg[e].from];
    bool flg = 1;
    //因为求的是起点边终点到终点边起点的距离，所以要加上起点站到起点边终点的距离和终点边起点到终点站的距离
    for (int i = 0; i < edg[s].sta.size(); i++)
    {
        if (edg[s].sta[i].str2 == S)
        {
            if (edg[s].to < 0)ans += edg[s].sta[i].lc1;//尽头线站
            else ans += edg[s].sta[i].lc2;//普通站
            break;
        }
    }
    for (int i = 0; i < edg[e].sta.size(); i++)
    {
        if (edg[e].sta[i].str2 == E)
        {
            if (edg[e].to < 0) ans += edg[e].sta[i].lc1;
            else ans += edg[s].sta[i].lc1;
            break;
        }
    }
    printf("最短里程:%d\n", ans);
    printf("经由路径:\n");
    vector<int> tmpv;
    for (int i = edg[e].from; i; i = pre[i])
    {
        tmpv.push_back(i);
    }
    reverse(tmpv.begin(), tmpv.end());
    flg = 0;
    if (edg[s].from != edg[s].to)//如果是节点站，tmpv[0]会输出S，避免重复输出
    {
        for (int i = 0; i < edg[s].sta.size(); i++)
        {
            if (flg || edg[s].sta[i].str2 == S)
            {
                flg = 1;
                cout << edg[s].sta[i].str2 << "->";
            }
        }
    }
    for (int i = 0; i < tmpv.size() - 1; i++)
    {
        for (int j = h[tmpv[i]]; j; j = nxt[j])
        {
            if (edg[j].from == edg[j].to)
            {
                for (int k = 0; k < edg[j].sta.size(); k++)
                {
                    cout << edg[j].sta[k].str2 << "->";
                }
                break;
            }
        }
        for (int j = h[tmpv[i]]; j; j = nxt[j])
        {
            if (edg[j].to == tmpv[i + 1])
            {
                for (int k = 0; k < edg[j].sta.size(); k++)
                {
                    cout << edg[j].sta[k].str2 << "->";
                }
                break;
            }
        }
    }
    //先把终点边的起点输出
    for (int i = h[edg[e].from]; i; i = nxt[i])
    {
        if (edg[i].from == edg[i].to)
        {
            for (int k = 0; k < edg[i].sta.size(); k++)
            {
                cout << edg[i].sta[k].str2;
                if (edg[i].sta[k].str2 == E)
                {
                    cout << "\n";
                    return;
                }
                else
                {
                    cout << "->";
                }
            }
        }
    }
    for (int i = 0; i < edg[e].sta.size(); i++)
    {
        cout << edg[e].sta[i].str2;
        if (edg[e].sta[i].str2 == E)
        {
            cout << "\n";
            return;
        }
        else
        {
            cout << "->";
        }
    }
}
int main()
{
    ifstream ifs;
    ifs.open("路网文件.txt", ios::in);
    int a, b, c;
    while (ifs >> a >> b >> c)
    {
        add1(a, b, c); add1(b, a, c);
    }
    ifs.close();
    ifs.open("站名文件.txt",ios::in);
    int n1, n2, lc1, lc2;
    string str1, str2;
    while (ifs >> n1 >> lc1 >> n2 >> lc2 >> str1 >> str2)
    {
        add2(n1, lc1, n2, lc2, str1, str2);
    }
    ifs.close();
    ofstream ofs;
    ofs.open("统计.txt", ios::out);
    int cntV = 0;
    for (int i = 1; i <= maxn; i++)
    {
        if (!h[i])continue;
        ofs << i << "\n";
        cntV++;
        for (int j = h[i]; j; j = nxt[j])
        {
            ofs << "   " << edg[j].to << ":\n";
            sort(edg[j].sta.begin(), edg[j].sta.end(), cmp);
            for (int k = 0; k < edg[j].sta.size(); k++)
            {
                ofs << "   " << edg[j].sta[k].lc1 << ' ' << edg[j].sta[k].lc2 << ' ' << edg[j].sta[k].str1 << ' ' << edg[j].sta[k].str2 << '\n';
            }
        }
    }
    ofs << cntV << '\n';
    ofs.close();
    printf("数据读取完成\n");
    printf("查询:输入出发站和终点站\n");
    //dijkstra("大理","大虎山");
    //dijkstra("社棠", "皮口");
    string S, E;
    while (cin >> S >> E)
    {
        dijkstra(S, E);
    }
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
