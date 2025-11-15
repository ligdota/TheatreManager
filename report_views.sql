/*
 * View #1: Ticket Sales
 * Provides details about ticket sales, combining ticket details, buyer information, seat assignments, and the related play.
 */
CREATE VIEW view_ticket_sales AS
SELECT t.ticket_id,
       pl.play_title,
       pa.patron_fname || ' ' || pa.patron_lname AS buyer_name,
       pa.patron_email AS buyer_email,
       s.seat_row || s.seat_num AS seat,
       t.ticket_cost,
       t.purchase_date
FROM ticket t
JOIN patron pa ON t.patron_id = pa.patron_id
JOIN seat s ON t.seat_id = s.seat_id
JOIN production p ON t.production_id = p.production_id
JOIN play pl ON p.play_id = pl.play_id;


/*
 * View #2: Production Cast and Crew
 * Combines information from members, roles, productions, and plays to show who is part of the cast/crew for each production.
 * */
CREATE VIEW view_production_cast_crew AS
SELECT p.production_id,
       pl.play_title,
       m.member_id,
       m.member_fname || ' ' || m.member_lname AS member_name,
       r.role_name
FROM member_role mr
JOIN member m ON mr.member_id = m.member_id
JOIN production p ON mr.production_id = p.production_id
JOIN play pl ON p.play_id = pl.play_id
JOIN role r ON mr.role_id = r.role_id;


/*
 * View #3: Member Dues
 * Listing of member dues status, showing whether each member has paid their fees.
 * */
CREATE VIEW view_member_dues AS
SELECT member_id,
       member_fname || ' ' || member_lname AS member_name,
       member_email,
       member_phone,
       CASE 
	       WHEN member_dues_paid 
	       THEN 'Paid' 
	       ELSE 'Unpaid' 
	   END AS dues_status
FROM member;




