/* sqlite sql, create eyesee db */


/* 1th table: create bg_post table */
CREATE TABLE IF NOT EXISTS bg_posts(
   post_id                 INTEGER NOT NULL,
   post_user_id            INTEGER,
   post_date               TEXT NOT NULL DEFAULT (datetime('now', 'localtime')),
   post_content            TEXT,
   post_title              TEXT,
   post_excerpt            TEXT,
   post_status             TEXT,
   comm_status             TEXT,
   pinged                  TEXT,
   ping_status             TEXT,
   to_ping                 TEXT,
   post_password           TEXT,
   post_name               TEXT,
   post_modified_date      TEXT,
   post_modified_date_gmt  TEXT,
   guid                    TEXT,
   post_parent             INTEGER,
   post_type               TEXT,
   post_mime_type          TEXT,
   comm_counts             INTEGER
);

/* 2th table: create bg_post_sequence */
CREATE TABLE IF NOT EXISTS bg_post_sequence(
	post_id		INTEGER PRIMARY KEY AUTOINCREMENT
);

/* 2th table: create bg_postmeta */
CREATE TABLE IF NOT EXISTS bg_postmeta(
   meta_id     INTEGER primary key,
   post_id     INTEGER,
   meta_key    TEXT,
   meta_value  TEXT
);

/* 3th table: create bg_comments */
CREATE TABLE IF NOT EXISTS bg_comments(
   comm_id           INTEGER primary key,
   comm_post_id      INTEGER,
   comm_author       TEXT,
   comm_author_email TEXT,
   comm_author_url   TEXT,
   comm_author_ip    TEXT,
   comm_date         TEXT NOT NULL DEFAULT (datetime('now', 'localtime')),
   comm_date_gmt     TEXT,
   comm_content      TEXT,
   comm_karma        TEXT,
   comm_state        TEXT,
   comm_agent        TEXT,
   comm_type         TEXT,
   comm_parent       INTEGER,
   user_id           INTEGER
);


/* 2th table: create bg_comm_sequence */
CREATE TABLE IF NOT EXISTS bg_comm_sequence(
	comm_id		INTEGER PRIMARY KEY AUTOINCREMENT
);


/* 4th table: create bg_commentmeta */
CREATE TABLE IF NOT EXISTS bg_commentmeta(
   meta_id     INTEGER primary key,
   comm_id     INTEGER,
   meta_key    TEXT,
   meta_value  TEXT
);


/* 5th table: create bg_users */
CREATE TABLE IF NOT EXISTS bg_users(
   user_id              INTEGER primary key,
   user_login           TEXT,
   user_pass            TEXT,
   user_nicename        TEXT,
   user_email           TEXT,
   user_url             TEXT,
   user_registered_date TEXT,
   user_activation_key  TEXT,
   user_status          TEXT,
   display_name         TEXT   
);

/* 6the table: create bg_usermeta */
CREATE TABLE IF NOT EXISTS bg_usermeta(
   meta_id     INTEGER primary key,
   user_id     INTEGER,
   meta_key    TEXT,
   meta_value  TEXT
);


/* 7th table: create bg_links */
CREATE TABLE IF NOT EXISTS bg_links(
   link_id              INTEGER primary key,
   link_url             TEXT,
   link_name            TEXT,
   link_image           TEXT,
   link_target          TEXT,
   link_description     TEXT,
   link_visible         TEXT,
   link_user_id         INTEGER,
   link_rating          TEXT,
   link_updated_date    TEXT,
   link_rel             TEXT,
   link_notes           TEXT
);


/* 8th table: create bg_term_relationships */
CREATE TABLE IF NOT EXISTS bg_term_relationships(
   object_id         INTEGER,
   term_taxonomy_id  INTEGER,
   term_order        INTEGER,
   primary key (object_id, term_taxonomy_id) 
);


/* 9th table: create bg_term_taxonomy */
CREATE TABLE IF NOT EXISTS bg_term_taxonomy(
   term_taxonomy_id  INTEGER primari key,
   term_id           INTEGER,
   taxonomy          TEXT,  /*分类体系的类别：post, link, tag */
   description       TEXT,
   parent            INTEGER,
   count             INTEGER
);

insert into bg_term_taxonomy( term_taxonomy_id, term_id, taxonomy, parent ) values( 1, 1, 'post', 0 );
insert into bg_term_taxonomy( term_taxonomy_id, term_id, taxonomy, parent ) values( 2, 2, 'post', 0 );
insert into bg_term_taxonomy( term_taxonomy_id, term_id, taxonomy, parent ) values( 3, 3, 'post', 0 );	
insert into bg_term_taxonomy( term_taxonomy_id, term_id, taxonomy, parent ) values( 4, 4, 'post', 0 );




/* 10th table: create bg_terms */
CREATE TABLE IF NOT EXISTS bg_terms(
   term_id     INTEGER primary key,
   name        TEXT,
   slug        TEXT,
   term_group  INTEGER,
   unique( term_id, name)
);

insert into bg_terms( term_id, name ) values(1, '技术');
insert into bg_terms( term_id, name ) values(2, '随笔');
insert into bg_terms( term_id, name ) values(3, '闲聊');
insert into bg_terms( term_id, name ) values(4, '未分');



/* 11th table: create bg_options */
CREATE TABLE IF NOT EXISTS bg_options(
   option_id      INTEGER primary key,
   blog_id        INTEGER,
   option_name    TEXT,
   option_value   TEXT,
   autoload       TEXT
);
