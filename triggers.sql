/* 
 * Trigger #1: Prevent double-booking of seats
 * Ensures a patron can't buy a seat that's already sold for the same production.
 */
CREATE TRIGGER prevent_double_booking
BEFORE INSERT ON ticket
BEGIN
    SELECT
        CASE
            WHEN EXISTS (
                SELECT 1
                FROM ticket
                WHERE seat_id = NEW.seat_id
                  AND production_id = NEW.production_id
            )
            THEN RAISE(ABORT, 'Seat already booked for this production')
        END;
END;


/* 
 * Trigger #2: Ensure unique member roles for each production
 * Prevents assigning the same role to a member for the same production more than once.
 */
CREATE TRIGGER unique_member_role
BEFORE INSERT ON member_role
FOR EACH ROW
BEGIN
    SELECT 
        CASE
            WHEN EXISTS (
                SELECT 1 
                FROM member_role
                WHERE member_id = NEW.member_id
                  AND production_id = NEW.production_id
                  AND role_id = NEW.role_id
            )
            THEN RAISE(ABORT, 'Member has already been assigned to this role.')
        END;
END;


/* 
 * Trigger #3: Ensure only one subscription per patron
 * Prevents a patron from having multiple subscriptions.
 */
CREATE TRIGGER single_patron_subscription
BEFORE INSERT ON patron
FOR EACH ROW
WHEN NEW.patron_subscription = 1 AND EXISTS (
        SELECT 1 
        FROM patron 
        WHERE patron_subscription = 1
        AND patron_phone = NEW.patron_phone
        AND patron_email = NEW.patron_email
    )
BEGIN
    SELECT RAISE(ABORT, 'This patron already has a subscription.');
END;
