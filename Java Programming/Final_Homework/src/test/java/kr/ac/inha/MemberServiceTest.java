package kr.ac.inha;

import kr.ac.inha.model.MemberVO;
import kr.ac.inha.service.MemberService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.annotation.Rollback;
import static org.assertj.core.api.Assertions.*;

@SpringBootTest
public class MemberServiceTest {
    @Autowired
    private MemberService memberService;

    @Test
//    @Rollback(false)
    void insertMember() throws Exception {
//        MemberVO memberVO = new MemberVO();
//        memberVO.setID("vltndud123");
//        memberVO.setPW("tndud2325!");
//        memberVO.setNickname("김두부맨");
//        memberVO.setName("피수영");
//        memberVO.setMail("auburn0820@naver.com");
//        memberVO.setHello("안녕하세용");
//
//        memberService.memberJoin(memberVO);

    }
}
