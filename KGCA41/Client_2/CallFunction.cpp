#include "Sample.h"
void   Sample::ChatMsg(UPACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str());
}
void   Sample::ChatNameReq(UPACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), L"�̸��� �Է��Ͻÿ� : ");
}
void   Sample::JoinUser(UPACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"���� �����Ͽ����ϴ�.");

}
void   Sample::NameAck(UPACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"��ȭ�� ��� ����");

}