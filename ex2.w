
declare
	
	var x ;
	proc p(a, b) is 
		declare 
			var w  
		begin 
			x := t  
		end ;
	proc q() is
		declare
			var w
		begin
			x := 0 
		end;
	proc r(z) is
		declare 
			var t
		begin
			t:=z+2*x ;
			call q()
		end 
	  

begin

	x := 1 	;
	call p (12, x+1, x*2 + 1, 12 + 4) ;
	call q () ;
	call r (0) ;
	if (x = 1 /\ x > z) then x := x+2 else z := 0
end
