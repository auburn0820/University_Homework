package kr.ac.inha.service;

import kr.ac.inha.mapper.MemberMapper;
import kr.ac.inha.model.MemberVO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class MemberServiceImpl implements MemberService {

    @Autowired
    MemberMapper membermapper;

    @Override
    public void memberJoin(MemberVO member) throws Exception {
        membermapper.memberJoin(member) ;
    }

    /* 아이디 중복 검사 */
    @Override
    public int idCheck(String memberId) throws Exception {
        return membermapper.idCheck(memberId);
    }

    /* 로그인 */
    @Override
    public int memberLogin(String memberID, String memberPW) throws Exception {
        return membermapper.memberLogin(memberID, memberPW);
    }

    @Override
    public List<MemberVO> memberSelectAll() throws Exception {
        return membermapper.memberSelectAll();
    }

    @Override
    public int checkDuplication(String memberID) throws Exception {
        return membermapper.idCheck(memberID);
    }

    @Override
    public boolean memberUpdate(MemberVO memberVO) throws Exception {
        return membermapper.memberUpdate(memberVO);
    }

    public boolean memberDelete(String memberID) throws Exception {
        return membermapper.memberDelete(memberID);
    }
}
