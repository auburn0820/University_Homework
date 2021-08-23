package kr.ac.inha.mapper;

import kr.ac.inha.model.MemberVO;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface MemberMapper {

    /* 회원가입 */
//    public String memberJoin(MemberVO member);
    public void memberJoin(MemberVO member);

    /* 로그인 */
    public int memberLogin(String memberID, String memberPW);

    /* 유저 정보 모두 가져오기 */
    public List<MemberVO> memberSelectAll();

    /* 아이디 중복 검사 */
    public int idCheck(String memberId);

    /* 유저 정보 변경 */
    public boolean memberUpdate(MemberVO memberVO);

    /* 유저 탈퇴 */
    public boolean memberDelete(String memberID);
}
