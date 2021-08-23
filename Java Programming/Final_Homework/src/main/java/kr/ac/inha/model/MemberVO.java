package kr.ac.inha.model;

public class MemberVO {
    private String ID;
    private String PW;
    private String NICKNAME;
    private String NAME;
    private String MAIL;
    private String HELLO;

    @Override
    public String toString() {
        return "MemberVO{" +
                "ID='" + ID + '\'' +
                ", PW='" + PW + '\'' +
                ", NICKNAME='" + NICKNAME + '\'' +
                ", NAME='" + NAME + '\'' +
                ", MAIL='" + MAIL + '\'' +
                ", HELLO='" + HELLO + '\'' +
                '}';
    }

    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }

    public String getPW() {
        return PW;
    }

    public void setPW(String PW) {
        this.PW = PW;
    }

    public String getNICKNAME() {
        return NICKNAME;
    }

    public void setNICKNAME(String NICKNAME) {
        this.NICKNAME = NICKNAME;
    }

    public String getNAME() {
        return NAME;
    }

    public void setNAME(String NAME) {
        this.NAME = NAME;
    }

    public String getMAIL() {
        return MAIL;
    }

    public void setMAIL(String MAIL) {
        this.MAIL = MAIL;
    }

    public String getHELLO() {
        return HELLO;
    }

    public void setHELLO(String HELLO) {
        this.HELLO = HELLO;
    }
}
