#include "Sample.h"
void   Sample::ChatMsg(UPACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str());
}
void   Sample::ChatNameReq(UPACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), L"이름을 입력하시오 : ");
}
void   Sample::JoinUser(UPACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"님이 입장하였습니다.");

}
void   Sample::NameAck(UPACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"대화명 사용 승인");

}