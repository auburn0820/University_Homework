package kr.ac.inha.service;

import kr.ac.inha.model.MemberVO;

import java.util.List;

public interface MemberService {

    /* 회원가입 */
//    public String memberJoin(MemberVO member) throws Exception;

    public void memberJoin(MemberVO member) throws Exception;

    /* 아이디 중복 검사 */
    public int idCheck(String memberId) throws Exception;

    /* 로그인 */
    public int memberLogin(String memberID, String memberPW) throws Exception;

    /* 유저 정보 가져오기 */
    public List<MemberVO> memberSelectAll() throws Exception;

    /* 아이디 중복 체크 */
    public int checkDuplication(String memberID) throws Exception;

    public boolean memberUpdate(MemberVO memberVO) throws Exception;

    public boolean memberDelete(String memberID) throws Exception;
}
