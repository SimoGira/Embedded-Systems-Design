PACKAGE xtea_pack IS
CONSTANT SIZE                        : INTEGER := 32;
CONSTANT Reset_ST                    : INTEGER := 0;
CONSTANT ST_0                        : INTEGER := 1;
CONSTANT ST_1                        : INTEGER := 2;
CONSTANT ST_2                        : INTEGER := 3;
CONSTANT ST_3                        : INTEGER := 4;
CONSTANT ST_4                        : INTEGER := 5;
CONSTANT ST_5                        : INTEGER := 6;
CONSTANT ST_6                        : INTEGER := 7;
CONSTANT ST_7                        : INTEGER := 8;
CONSTANT ST_8                        : INTEGER := 9;
CONSTANT ST_9                        : INTEGER := 10;
CONSTANT ST_10                       : INTEGER := 11;
CONSTANT ST_11                       : INTEGER := 12;
CONSTANT ST_12                       : INTEGER := 13;
CONSTANT ST_13                       : INTEGER := 14;
CONSTANT ST_14                       : INTEGER := 15;
CONSTANT ST_15                       : INTEGER := 16;

END xtea_pack;

LIBRARY IEEE;
USE IEEE.std_logic_1164.ALL;
USE WORK.xtea_pack.ALL;
USE IEEE.NUMERIC_BIT.ALL;

--USE IEEE.STD_LOGIC_ARITH.ALL;
--USE IEEE.NUMERIC_STD.ALL;
--USE IEEE.STD_LOGIC_UNSIGNED.ALL;

entity xtea is
  port (	clk, rst             : in  bit;
  din_rdy                            : in  bit;
  word0_in, word1_in                 : in  UNSIGNED (SIZE-1 DOWNTO 0);
  --key0_in, key1_in, key2_in, key3_in : in  UNSIGNED (SIZE-1 DOWNTO 0);
  mode_in                            : in  bit;

  --result0_out, result1_out           : out UNSIGNED (SIZE-1 DOWNTO 0);
  result_out                        : out UNSIGNED (SIZE-1 DOWNTO 0);
  dout_rdy                           : out bit
  ) ;
end xtea;

architecture xtea of xtea is
  subtype status_t is integer range 0 to 16;
  subtype internal_t is UNSIGNED (SIZE-1 DOWNTO 0);
  signal STATUS                      : status_t;
  signal NEXT_STATUS                 : status_t;

  signal mode                        : bit;
  signal i, delta, v0, v1, temp      : internal_t;
  signal key0, key1, key2, key3      : internal_t;
  signal sum                         : UNSIGNED (SIZE*2-1 DOWNTO 0);
  signal v64                         : UNSIGNED (SIZE*2-1 DOWNTO 0);

  CONSTANT ZERO                      : internal_t       := "00000000000000000000000000000000";
  CONSTANT UNO                       : internal_t       := "00000000000000000000000000000001";
  CONSTANT DUE                       : internal_t       := "00000000000000000000000000000010";
  CONSTANT TRE                       : internal_t       := "00000000000000000000000000000011";
  CONSTANT UNDICI                    : internal_t       := "00000000000000000000000000001011";
  CONSTANT SEDICI                    : internal_t       := "00000000000000000000000000010000";




  begin

    -- elaborate_SQRT_FSM
    process(STATUS, din_rdy, mode_in, i)

    begin
      case STATUS is
        when Reset_ST =>
        NEXT_STATUS <= ST_0;
        when ST_0 =>
        if din_rdy = '1' then
          NEXT_STATUS <= ST_1;
        else
          NEXT_STATUS <= ST_0;
        end if;
        when ST_1   =>
          NEXT_STATUS <= ST_13;
        when ST_13  =>
          NEXT_STATUS <= ST_14;
        when ST_14  =>
        if mode_in = '0' then
          NEXT_STATUS <= ST_2;
        else
          NEXT_STATUS <= ST_7;
        end if;
        when ST_2   =>
        if i < 32 then
          NEXT_STATUS <= ST_3;
        else
          NEXT_STATUS <= ST_6;
        end if;
        when ST_3  =>
        NEXT_STATUS <= ST_4;
        when ST_4  =>
        NEXT_STATUS <= ST_5;
        when ST_5  =>
        NEXT_STATUS <= ST_2;
        when ST_6  =>
        NEXT_STATUS <= ST_15;
	when ST_15 =>
	NEXT_STATUS <= ST_0; 
        when ST_7  =>
        NEXT_STATUS <= ST_8;
        when ST_8  =>
        NEXT_STATUS <= ST_9;
        when ST_9  =>
        if i < 32 then
          NEXT_STATUS <= ST_10;
        else
          NEXT_STATUS <= ST_6;
        end if;
        when ST_10  =>
        NEXT_STATUS <= ST_11;
        when ST_11  =>
        NEXT_STATUS <= ST_12;
        when ST_12  =>
        NEXT_STATUS <= ST_9;
        when others =>
        NEXT_STATUS <= STATUS;
      end case;
    end process;


    -- elaborate_SQRT
    process(clk, rst)

    begin
      if rst='0' then
        STATUS <= Reset_ST;
      elsif clk'event and clk='1' then
        STATUS <= NEXT_STATUS;
        case NEXT_STATUS is
          when Reset_ST =>
          result_out <= ZERO;
          --result1_out <= ZERO;
          dout_rdy <= '0';
          when ST_0 =>
          result_out <= ZERO;
          --result1_out <= ZERO;
          dout_rdy <= '0';

          i    <= ZERO;
          temp <= ZERO;
          delta<= ZERO;
          sum  <= ZERO & ZERO;
          when ST_1   =>
          v0 <= word0_in;
          v1 <= word1_in;
          -- key0 <= key0_in;
          -- key1 <= key1_in;
          -- key2 <= key2_in;
          -- key3 <= key3_in;
          mode <= mode_in;

          when ST_13  =>
          key0 <= word0_in;
          key1 <= word1_in;

          when ST_14  =>
          key2 <= word0_in;
          key3 <= word1_in;

          when ST_2   =>
          delta <= x"9e3779b9";

          if (sum and (ZERO & TRE)) = (ZERO & ZERO) then
            temp <= key0;
          elsif (sum and (ZERO & TRE)) = (ZERO & UNO) then
            temp <= key1;
          elsif (sum and (ZERO & TRE)) = (ZERO & DUE) then
            temp <= key2;
          else temp <= key3;
        end if;
        when ST_3  =>
        v0 <= v0 + ((((v1 sll 4) xor (v1 srl 5)) + v1) xor (sum(31 downto 0) + temp));
        sum <= sum + (ZERO & delta);
        when ST_4  =>
        if ((sum srl 11) and (ZERO & TRE)) = (ZERO & ZERO) then
          temp <= key0;
        elsif ((sum srl 11) and (ZERO & TRE)) = (ZERO & UNO) then
          temp <= key1;
        elsif ((sum srl 11) and (ZERO & TRE)) = (ZERO & DUE) then
          temp <= key2;
        else temp <= key3;
      end if;
      when ST_5  =>
      v1 <= v1 + ((((v0 sll 4) xor (v0 srl 5)) + v0) xor (sum(31 downto 0) + temp));
      i <= i + UNO;
      when ST_6  =>
      result_out <= v0;
      --result1_out <= v1;
      dout_rdy <= '1';
      when ST_15 =>
      result_out <= v1;
      when ST_7  =>
      delta <= x"9e3779b9";
      when ST_8  =>
      sum <= delta * 32;
      when ST_9  =>
      if ((sum srl 11) and (ZERO & TRE)) = (ZERO & ZERO) then
        temp <= key0;
      elsif ((sum srl 11) and (ZERO & TRE)) = (ZERO & UNO) then
        temp <= key1;
      elsif ((sum srl 11) and (ZERO & TRE)) = (ZERO & DUE) then
        temp <= key2;
      else temp <= key3;
    end if;
    when ST_10  =>
    v1 <= v1 - ((((v0 sll 4) xor (v0 srl 5)) + v0) xor (sum(31 downto 0) + temp));
    sum <= sum - (ZERO & delta);
    when ST_11  =>
    if (sum and (ZERO & TRE)) = (ZERO & ZERO) then
      temp <= key0;
    elsif (sum and (ZERO & TRE)) = (ZERO & UNO) then
      temp <= key1;
    elsif (sum and (ZERO & TRE)) = (ZERO & DUE) then
      temp <= key2;
    else temp <= key3;
  end if;
  when ST_12  =>
  v0 <= v0 - ((((v1 sll 4) xor (v1 srl 5)) + v1) xor (sum(31 downto 0) + temp));
  i <= i + UNO;
end case;
end if;
end process;

end xtea;
