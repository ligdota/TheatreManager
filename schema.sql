/*

Tables and their relationships:

- play: Catalog of plays that the company can produce.
- member: People involved in productions (actors, crew, producers, etc.).
- role: Roles that a member can have in a production (e.g., Actor, Director).
- production: A specific staging of a play on a particular date, with ticket price and producer.
- member_role: Weak entity linking member, role, and production (who did what in which production).
- patron: Customers who buy tickets or have subscriptions.
- seat: Seats in the venue (row/seat number).
- sponsor: Organizations or individuals providing sponsorship.
- transaction_type: Lookup of finance transaction categories (member dues, ticket sales, etc.).
- finances: Financial transactions associated with a production, typed via transaction_type.
- ticket: Tickets sold to patrons for specific productions and seats.
- sponsor_donations: Weak entity capturing donations from sponsors towards productions.

Foreign key relationships:

- production.play_id              -> play.play_id
- production.producer_id          -> member.member_id
- member_role.role_id             -> role.role_id
- member_role.member_id           -> member.member_id
- member_role.production_id       -> production.production_id
- finances.production_id          -> production.production_id
- finances.transaction_type_id    -> transaction_type.transaction_type_id
- ticket.production_id            -> production.production_id
- ticket.patron_id                -> patron.patron_id
- ticket.seat_id                  -> seat.seat_id
- sponsor_donations.sponsor_id    -> sponsor.sponsor_id
- sponsor_donations.production_id -> production.production_id


On update clauses.
When a play is deleted, all associated productions are deleted.
When a member (producer) is deleted, all associated productions are deleted.
When a role is deleted, all associated member_role entries are deleted.
When a member is deleted, all associated member_role entries are deleted.
When a production is deleted, all associated member_role entries are deleted.
When a production is deleted, all associated tickets are deleted.

*/


PRAGMA foreign_keys = ON;


-- lookup table for different types of financial transactions
CREATE TABLE transaction_type (
    transaction_type_id INTEGER PRIMARY KEY AUTOINCREMENT, -- surrogate
    transaction_type_name TEXT  UNIQUE NOT NULL -- readable name
);

-- data for transaction types to be used in the schema
INSERT INTO transaction_type (transaction_type_name) VALUES
('member_dues'),
('sponsor_ad_creds'),
('sponsor_prod_creds'),
('ticket_sales'),
('expenses');

-- data for plays that can be produced by the theatre, each row is distinct
CREATE Table play (
play_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique id for each play
play_title TEXT  NOT NULL UNIQUE, -- title, must be unique
play_author TEXT  NOT NULL, -- playwright
play_genre TEXT  NOT NULL, -- comedy, tragedy, drama, etc..
play_num_acts INTEGER NOT NULL -- number of acts in the script
);  


-- specific production of a play on a given date, links a play to a producer and stores ticket price
CREATE Table production (
  production_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique production instance
  play_id INTEGER NOT NULL REFERENCES play(play_id) ON DELETE CASCADE, -- which play is being produced
  producer_id INTEGER NOT NULL REFERENCES member(member_id) ON DELETE CASCADE, -- the producer in charge
  production_ticket_price decimal NOT NULL, -- base ticket price for this production
  production_date date -- date of the play
);


-- members of the theatre company (actors, directors, crew, etc..)
CREATE Table member (
member_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique name id
member_fname TEXT NOT NULL, -- first name
member_lname TEXT NOT NULL, -- last name
member_email TEXT NOT NULL, -- contact email
member_phone TEXT NOT NULL, -- contact phone number
member_dues_paid bool -- flag indicating if dues are current
);

-- role catalog
CREATE Table role (
role_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique role id
role_name TEXT NOT NULL UNIQUE -- unique name for the role
);

--- note - memberRole is a weak entity dependent on role, production, and member
-- connects a member to a role in a specific production (primary key is the combination of member and production id)
CREATE Table member_role (
  member_id INTEGER NOT NULL REFERENCES member(member_id) ON DELETE CASCADE, -- who
  production_id INTEGER NOT NULL REFERENCES production(production_id) ON DELETE CASCADE, -- in what production
  role_id INTEGER NOT NULL REFERENCES role(role_id) ON DELETE CASCADE, -- with what role
  PRIMARY KEY (member_id, production_id, role_id) -- composite key to ensure uniqueness
);


-- customers who buy tickets or hold subscriptions
CREATE Table patron (
  patron_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique id
  patron_fname TEXT NOT NULL, -- first name
  patron_lname TEXT NOT NULL, -- last name
  patron_email TEXT NOT NULL, -- contact email
  patron_phone TEXT NOT NULL, -- contact phone number
  patron_address TEXT  NOT NULL, -- mailing address
  patron_subscription BOOLEAN -- indicates if a patron has a sub
);

-- seats in the venue, each set identified by row and seat number
CREATE Table seat (
  seat_id INTEGER PRIMARY KEY AUTOINCREMENT, -- surrogate key for the seat
  seat_row char NOT NULL, -- row identifier ('A', 'B', etc..)
  seat_num INTEGER NOT NULL -- seat number within the row
);

-- sponsors who provide financial support
CREATE Table sponsor (
  sponsor_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique id
  sponsor_name TEXT NOT NULL, -- organization/person name
  sponsor_phone TEXT NOT NULL, -- contact phone number
  sponsor_email TEXT NOT NULL -- contact email
);

-- financial transactions specifically associated with productions (uses transaction id to categorize)
CREATE Table finances (
  transaction_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique id
  production_id INTEGER NOT NULL REFERENCES production(production_id) ON DELETE CASCADE, -- which productiton does this transaction relate to
  transaction_type_id INTEGER NOT NULL REFERENCES transaction_type(transaction_type_id), -- category of transactiom
  transaction_amount DECIMAL NOT NULL, -- positive/negative amount
  transaction_date DATE NOT NULL -- when the transaction occured
);

-- tickets sold to patrons for seats in specific productions
CREATE Table ticket (
  ticket_id INTEGER PRIMARY KEY AUTOINCREMENT, -- unique id
  production_id INTEGER NOT NULL REFERENCES production(production_id) ON DELETE CASCADE, -- which production
  patron_id INTEGER NOT NULL REFERENCES patron(patron_id), -- which patron purchased it
  seat_id INTEGER NOT NULL REFERENCES seat(seat_id), -- which seat is reserved
  ticket_cost decimal NOT NULL, -- price paid for ticket
  purchase_date date NOT NULL -- date of ticket purchase
);

-- connects sponsors with productions using donation amounts and credits
CREATE Table sponsor_donations (
  sponsor_id INTEGER NOT NULL REFERENCES sponsor(sponsor_id), -- sponsor making the donation
  production_id INTEGER NOT NULL REFERENCES production(production_id), -- production being sponsored
  donation_amount decimal NOT NULL, -- total donation amount
  sponsor_ad_creds decimal, -- production credits granted
  sponsor_prod_creds decimal, --weak composite key
  PRIMARY KEY (sponsor_id, production_id)
  --- note - sponsorDonations is a weak entity dependent on sponsor and production
);



  








  





