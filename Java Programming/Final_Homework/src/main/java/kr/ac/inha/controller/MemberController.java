package kr.ac.inha.controller;

import kr.ac.inha.service.MemberService;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import kr.ac.inha.model.MemberVO;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpSession;
import java.util.List;


@RestController
@MapperScan(basePackages = "kr.ac.inha.dao")
public class MemberController {
    @Autowired
    private MemberService memberService;

    private String sessionId = null;

    @RequestMapping("/welcome.do")
    public ModelAndView welcome() {
        ModelAndView mv = new ModelAndView();
        mv.setViewName("welcome");
        if(sessionId != null) {
            mv.addObject("sessionId", sessionId);
        }
        return mv;
    }

    @RequestMapping("/member/deleteMember.do")
    public ModelAndView deleteMember() throws Exception {
        if (memberService.idCheck(sessionId) < 1) {
            ModelAndView modelAndView = new ModelAndView();
            modelAndView.setViewName("redirect:updateMember.do");
            return modelAndView;
        }

        memberService.memberDelete(sessionId);

        sessionId = null;
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("/welcome");
        modelAndView.addObject("sessionId", null);

        return modelAndView;
    }

    @RequestMapping("/member/addMember.do")
    public ModelAndView addMember() {
        ModelAndView mv = new ModelAndView();
        mv.setViewName("member/addMember");
        return mv;
    }

    @RequestMapping("/member/updateMember.do")
    public ModelAndView updateMember() throws Exception {
        ModelAndView mv = new ModelAndView();
        mv.setViewName("member/updateMember");

        if(sessionId != null) {
            mv.addObject("sessionId", sessionId);
        }

        return mv;
    }

    @RequestMapping("/member/processUpdateMember.do")
    public ModelAndView updateMemberProcess(String id, String pw, String nickname, String name, String mail, String hello) throws Exception {
        MemberVO memberVO = new MemberVO();

        memberVO.setID(id);
        memberVO.setPW(pw);
        memberVO.setNICKNAME(nickname);
        memberVO.setNAME(name);
        memberVO.setMAIL(mail);
        memberVO.setHELLO(hello);

        if (memberService.idCheck(id) < 1) {
            ModelAndView modelAndView = new ModelAndView();
            modelAndView.setViewName("redirect:updateMember.do");

            return modelAndView;
        }

        memberService.memberUpdate(memberVO);

        ModelAndView modelAndView = new ModelAndView();
        modelAndView.addObject("sessionId", id);
        modelAndView.setViewName("/welcome");

        return modelAndView;
    }

    @RequestMapping(value = "/member/processAddMember.do", method = RequestMethod.POST)
    public ModelAndView joinMember(String id, String pw, String nickname, String name, String mail, String hello) throws Exception {
        MemberVO member = new MemberVO();

        member.setID(id);
        member.setPW(pw);
        member.setNICKNAME(nickname);
        member.setNAME(name);
        member.setMAIL(mail);
        member.setHELLO(hello);

        int result = memberService.idCheck(member.getID());

        if (result != 0) {
            ModelAndView modelAndView = new ModelAndView();
            modelAndView.setViewName("redirect:addMember.do");
            modelAndView.addObject("message", "중복된 아이디입니다.");
            return modelAndView;
        }

        memberService.memberJoin(member);

        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("redirect:loginMember.do");
        return modelAndView;
    }

    @RequestMapping("/member/loginMember.do")
    public ModelAndView loginMember() throws Exception {
        ModelAndView modelAndView = new ModelAndView();
        modelAndView.setViewName("member/loginMember");

        return modelAndView;
    }

    @RequestMapping("/member/processLoginMember.do")
    public ModelAndView processLoginMember(String id, String pw) throws Exception {
        int result = memberService.memberLogin(id, pw);

        if (result >= 1) {
            ModelAndView mv = new ModelAndView();
            mv.setViewName("/welcome");
            mv.addObject("sessionId", id);
            sessionId = id;

            return mv;
        }

        ModelAndView mv = new ModelAndView();
        mv.setViewName("member/loginMember");
        return mv;
    }

    @RequestMapping("/member/logoutMember.do")
    public ModelAndView logoutAddMember() {
        ModelAndView mv = new ModelAndView();
        mv.setViewName("welcome");
        mv.addObject("sessionID", null);
        sessionId = null;

        return mv;
    }
}
