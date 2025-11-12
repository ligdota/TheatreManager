

/*

List all foreign keys
production.play_id -> play.play_id
production.producer_id -> member.member_id
member_role.role_id -> role.role_id
member_role.member_id -> member.member_id
member_role.production_id -> production.production_id
finances.production_id -> production.production_id
ticket.production_id -> production.production_id
ticket.patron_id -> patron.patron_id
sponsor_donations.sponsor_id -> sponsor.sponsor_id
sponsor_donations.production_id -> production.production_id
ticket.seat_id -> seat.seat_id


On update clauses.
When a play is deleted, all associated productions are deleted.
When a member (producer) is deleted, all associated productions are deleted.
When a role is deleted, all associated member_role entries are deleted.
When a member is deleted, all associated member_role entries are deleted.
When a production is deleted, all associated member_role entries are deleted.
When a production is deleted, all associated tickets are deleted.


*/


PRAGMA foreign_keys = ON;



CREATE TABLE transaction_type (
    transaction_type_id INTEGER PRIMARY KEY AUTOINCREMENT,
    transaction_type_name TEXT  UNIQUE NOT NULL
);

INSERT INTO transaction_type (transaction_type_name) VALUES
('member_dues'),
('sponsor_ad_creds'),
('sponsor_prod_creds'),
('ticket_sales'),
('expenses');


CREATE Table play (
play_id INTEGER PRIMARY KEY AUTOINCREMENT,
play_title TEXT  NOT NULL UNIQUE,
play_author TEXT  NOT NULL,
play_genre TEXT  NOT NULL,
play_num_acts INTEGER NOT NULL
);  


CREATE Table production (
  production_id INTEGER PRIMARY KEY AUTOINCREMENT,
  play_id INTEGER NOT NULL REFERENCES play(play_id) ON DELETE CASCADE,
  producer_id INTEGER NOT NULL REFERENCES member(member_id) ON DELETE CASCADE,
  production_ticket_price decimal NOT NULL,
  production_date date
);



CREATE Table member (
member_id INTEGER PRIMARY KEY AUTOINCREMENT,
member_fname TEXT NOT NULL,
member_lname TEXT NOT NULL,
member_email TEXT NOT NULL,
member_phone TEXT NOT NULL,
member_dues_paid bool
);

CREATE Table role (
role_id INTEGER PRIMARY KEY AUTOINCREMENT,
role_name TEXT NOT NULL UNIQUE
);

--- note - memberRole is a weak entity dependent on role, production, and member
CREATE Table member_role (
  member_id INTEGER NOT NULL REFERENCES member(member_id) ON DELETE CASCADE,
  production_id INTEGER NOT NULL REFERENCES production(production_id) ON DELETE CASCADE,
  role_id INTEGER NOT NULL REFERENCES role(role_id) ON DELETE CASCADE,
  PRIMARY KEY (member_id, production_id, role_id)
);



CREATE Table patron (
  patron_id INTEGER PRIMARY KEY AUTOINCREMENT,
  patron_fname TEXT NOT NULL,
  patron_lname TEXT NOT NULL,
  patron_email TEXT NOT NULL,
  patron_phone TEXT NOT NULL,
  patron_address TEXT  NOT NULL,
  patron_subscription BOOLEAN  
);

CREATE Table seat (
  seat_id INTEGER PRIMARY KEY AUTOINCREMENT,
  seat_row char NOT NULL,
  seat_num INTEGER NOT NULL
);

CREATE Table sponsor (
  sponsor_id INTEGER PRIMARY KEY AUTOINCREMENT,
  sponsor_name TEXT NOT NULL,
  sponsor_phone TEXT NOT NULL,
  sponsor_email TEXT NOT NULL
);


CREATE Table finances (
  transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
  production_id INTEGER NOT NULL REFERENCES production(production_id) ON DELETE CASCADE,
  transaction_type_id INTEGER NOT NULL REFERENCES transaction_type(transaction_type_id),
  transaction_amount DECIMAL NOT NULL,
  transaction_date DATE NOT NULL
);


CREATE Table ticket (
  ticket_id INTEGER PRIMARY KEY AUTOINCREMENT,
  production_id INTEGER NOT NULL REFERENCES production(production_id) ON DELETE CASCADE,
  patron_id INTEGER NOT NULL REFERENCES patron(patron_id),
  seat_id INTEGER NOT NULL REFERENCES seat(seat_id),
  ticket_cost decimal NOT NULL,
  purchase_date date NOT NULL
);


CREATE Table sponsor_donations (
  sponsor_id INTEGER NOT NULL REFERENCES sponsor(sponsor_id),
  production_id INTEGER NOT NULL REFERENCES production(production_id),
  donation_amount decimal NOT NULL,
  sponsor_ad_creds decimal,
  sponsor_prod_creds decimal,
  PRIMARY KEY (sponsor_id, production_id)
  --- note - sponsorDonations is a weak entity dependent on sponsor and production
);



  








  





