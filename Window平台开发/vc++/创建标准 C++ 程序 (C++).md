������׼ C++ ���� (C++)

������Ŀ�����Դ�ļ�
1.ͨ�����·�ʽ����һ����Ŀ��ָ���ļ����˵��ϵġ��½�����Ȼ�󵥻�����Ŀ����

2.�ڡ�Visual C++����Ŀ���ʹ����У�������Win32����Ȼ�󵥻���Win32 ����̨Ӧ�ó��򡱡�

3.������Ŀ���ơ�

Ĭ������£�������Ŀ�Ľ����������Ŀͬ�����������Լ����������ơ� ��Ҳ����Ϊ��Ŀ��������λ�á� 

������ȷ����������Ŀ��

4.�ڡ�Win32 Ӧ�ó����򵼡��У���������һ������ѡ�񡰿���Ŀ����Ȼ�󵥻�����ɡ���

5.���δ��ʾ�����������Դ�������������ڡ���ͼ���˵��ϣ����������������Դ����������

6.��һ����Դ�ļ���ӵ���Ŀ��������ʾ��

a.�ڡ����������Դ���������У��һ���Դ�ļ����ļ��У�ָ����ӡ���Ȼ�󵥻����½����

b.�ڡ����롱�ڵ��е�����C++ �ļ�(.cpp)����Ϊ�ļ��������ƣ�Ȼ�󵥻�����ӡ���

�� .cpp �ļ�����ʾ�ڡ����������Դ���������еġ�Դ�ļ����ļ����У������ļ����� Visual Studio �༭���д򿪡�

7.�ڱ༭���ڵ��ļ��У�����ʹ�ñ�׼ C++ �����Ч C++ ���򣬻��߸���ʾ������֮һ������ճ�����ļ��С�

���磬������ʹ�� set::find (STL Samples)ʾ�����򣬸ó����ǰ����и����ı�׼ģ���ʾ��֮һ��
// SetFind.cpp
// compile with: /EHsc
//
//      Illustrates how to use the find function to get an iterator
//      that points to the first element in the controlled sequence
//      that has a particular sort key.
//
// Functions:
//
//    find         Returns an iterator that points to the first element
//                 in the controlled sequence that has the same sort key
//                 as the value passed to the find function. If no such
//                 element exists, the iterator equals end().
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)
#include <set>
#include <iostream>

using namespace std ;

typedef set<int> SET_INT;

void truefalse(int x)
{
  cout << (x?"True":"False") << endl;
}

int main() {
  SET_INT s1;
  cout << "s1.insert(5)" << endl;
  s1.insert(5);
  cout << "s1.insert(8)" << endl;
  s1.insert(8);
  cout << "s1.insert(12)" << endl;
  s1.insert(12);

  SET_INT::iterator it;
  cout << "it=find(8)" << endl;
  it=s1.find(8);
  cout << "it!=s1.end() returned ";
  truefalse(it!=s1.end());  //  True

  cout << "it=find(6)" << endl;
  it=s1.find(6);
  cout << "it!=s1.end() returned ";
  truefalse(it!=s1.end());  // False
}
/////////////////////////////////////////////////

���ʹ�ø�ʾ��������ע�� using namespace std; ָ� ��ָ��ʹ�����ܹ�ʹ�� cout �� endl����������ȫ�޶�����std::cout �� std::endl���� 

8.������ļ���

9.�ڡ����ɡ��˵��ϣ����������ɽ����������

�������������ʾ�йر�����̵���Ϣ�����磬������־��λ�ã��Լ�ָʾ����״̬����Ϣ��

10.�ڡ����ԡ��˵��ϣ���������ʼִ��(������)����

���ʹ����ʾ�����򣬽���ʾһ������ڣ�������ʾ�Ƿ��ڼ������ҵ����ض���������

