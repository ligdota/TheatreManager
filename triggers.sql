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
