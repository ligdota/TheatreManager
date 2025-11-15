
/*
 * These reports will use the approriate views to retrieve the results.
 * 
 * */

-- Retrieves a list of ticket sales from the view_ticket_sales view.
SELECT play_title, buyer_name, buyer_email, seat, ticket_cost, purchase_date
FROM view_ticket_sales
ORDER BY play_title;

-- Retrieves a list of production cast and crew from the view_production_cast_crew view.
SELECT production_id, play_title, member_name, role_name
FROM view_production_cast_crew
ORDER BY production_id, role_name, member_name;

-- Retrieves a list of member dues status and details from the view_member_dues view.
SELECT member_name, member_email, member_phone, dues_status
FROM view_member_dues
ORDER BY dues_status, member_name;