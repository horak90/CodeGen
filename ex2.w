
declare
	
	var x ;
	proc p(a, b) is 
		declare 
			var w  
		begin 
			x := a*b + t*2
		end ;
	proc q() is
		declare
			var w
		begin
			x := x + 1
		end;
	proc r(z) is
		declare 
			var t
		begin
			t:=z+2*x ;
			call q()
		end 
	  

begin
  x := 1 ;
	call p (12, x+1) ;
  call q () ;
  call r (0) ;
  if (x => 4 /\ x => z) then x := 0 else z := 0 ;
  while ^(x = 4) do
    call q ()
  od
end
