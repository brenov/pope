#include "services/pcg/peg.hpp"
#include <ctime>
#include "commons/common.hpp"

std::list<Entrant*> PEG::generate(YAML::Node database, int n) {
  std::list<Entrant*> entrants;
  // Generate entrants
  for (int i = 0; i < n; i++) {
    entrants.push_back(new_entrant(database));
  }
  // Shuffle list
  shuffle(entrants);
  return entrants;
}

Entrant* PEG::new_entrant(YAML::Node database) {
  // Random variables
  std::mt19937 rng;
  RandomInt randi;
  // Sex
  randi = RandomInt(0, 1);
  rng.seed(std::random_device()());
  char sex = database["sex"][randi(rng)].as<char>();
  // Gender-dependent variables
  std::string firstname;
  std::string hair;
  std::string facial_hair;
  if (sex == MALE) {
    // Male hair
    randi = RandomInt(0, database["male_hair"].size() - 1);
    rng.seed(std::random_device()());
    hair = database["male_hair"][randi(rng)].as<std::string>();
    // Facial hair
    randi = RandomInt(0, database["facial_hair"].size() - 1);
    rng.seed(std::random_device()());
    facial_hair = database["facial_hair"][randi(rng)].as<std::string>();
    // Male firstnames
    randi = RandomInt(0, database["male_firstnames"].size() - 1);
    rng.seed(std::random_device()());
    firstname = database["male_firstnames"][randi(rng)].as<std::string>();
  } else {
    // Female hair
    randi = RandomInt(0, database["female_hair"].size() - 1);
    rng.seed(std::random_device()());
    hair = database["female_hair"][randi(rng)].as<std::string>();
    // Female firstnames
    randi = RandomInt(0, database["female_firstnames"].size() - 1);
    rng.seed(std::random_device()());
    firstname = database["female_firstnames"][randi(rng)].as<std::string>();
  }
  // Vision
  randi = RandomInt(0, database["vision"].size() - 1);
  rng.seed(std::random_device()());
  std::string vision = database["vision"][randi(rng)].as<std::string>();
  // Other physical appearance
  randi = RandomInt(0, database["other"].size() - 1);
  rng.seed(std::random_device()());
  std::string other = database["other"][randi(rng)].as<std::string>();
  // Picture
  Picture pic(hair, facial_hair, vision, other);
  // Lastnames
  randi = RandomInt(0, database["lastnames"].size() - 1);
  rng.seed(std::random_device()());
  std::string lastname = database["lastnames"][randi(rng)].as<std::string>();
  // Date of birth
  time_t dmin = -1262304000;
  time_t dmax = -189475200;
  randi = RandomInt(dmin, dmax);
  time_t gd = randi(rng);
  struct tm* tm = localtime(&gd);
  Date date_of_birth(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
  // Height
  rng.seed(std::random_device()());
  randi = RandomInt((int) (database["min_height"].as<float>() * 100),
    (int) (database["max_height"].as<float>() * 100));
  float height = (randi(rng) / (float) 100);
  // Weight
  randi = RandomInt(database["min_weight"].as<int>(),
    database["max_weight"].as<int>());
  rng.seed(std::random_device()());
  unsigned int weight = randi(rng);
  // Country
  randi = RandomInt(0, database["countries"].size() - 1);
  rng.seed(std::random_device()());
  std::string country = database["countries"][randi(rng)].as<std::string>();
  // City
  randi = RandomInt(0, database["cities"][country].size() - 1);
  rng.seed(std::random_device()());
  std::string city = database["cities"][country][randi(rng)].as<std::string>();
  // Purpose
  randi = RandomInt(0, database["purpose"].size() - 1);
  rng.seed(std::random_device()());
  std::string purpose = database["purpose"][randi(rng)].as<std::string>();

  //// Illegal entrant
  // Illegal variables
  int invalid_fields;
  unsigned int bribe;
  std::list<std::string> contraband;
  // Is illegal?
  rng.seed(std::random_device()());
  randi = RandomInt(0, 4);
  bool illegal = (randi(rng) == 0);
  // If the entrant is illegal
  if (illegal) {
    // Amount of invalid fields
    randi = RandomInt(1, 5);
    rng.seed(std::random_device()());
    invalid_fields = randi(rng);
    // Amount of bribe
    randi = RandomInt(0, 4);
    rng.seed(std::random_device()());
    bribe = randi(rng) * 5;
    // If the entrant has contraband
    rng.seed(std::random_device()());
    randi = RandomInt(0, 3);
    if (randi(rng) == 0) {
      randi = RandomInt(0, database["contraband"].size() - 1);
      rng.seed(std::random_device()());
      contraband.push_back(database["contraband"][randi(rng)].as<std::string>());
    }
  }

  // Generate entrant's papers
  std::list<Paper*> papers;
  // Create entrant
  Entrant* entrant = new Entrant(pic, firstname, lastname, sex, date_of_birth,
    height, weight, country, city, papers, purpose, bribe, contraband);
  // Passport
  Passport* passport = new_passport(database, entrant, illegal, invalid_fields);
  entrant->papers.push_back(passport);
  // If the entrant is a native
  if (country == "arstotzka") {
    // ID Card
    entrant->papers.push_back(new_idcard(database, entrant, illegal,
      invalid_fields));
    purpose = "";
  } else {
    // Access permit
    AccessPermit* accesspermit = new_accesspermit(database, entrant, passport,
      illegal, invalid_fields);
    entrant->papers.push_back(accesspermit);
    if (purpose == "Work") {
      // Work pass
      entrant->papers.push_back(new_workpass(database, entrant, accesspermit,
        illegal, invalid_fields));
    }
  }
  return entrant;
}

Passport* PEG::new_passport(YAML::Node database, Entrant* entrant, bool illegal,
  int& invalid_fields) {
    Passport* passport;
    // Random variables
    std::mt19937 rng;
    RandomInt randi;

    std::string firstname = entrant->firstname;
    std::string lastname = entrant->lastname;
    char sex = entrant->sex;
    Date date_of_birth = entrant->date_of_birth;
    std::string country = entrant->country;
    char s[10];
    static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 10; ++i) {
      s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    std::string passport_number(s);
    // Seal
    randi = RandomInt(0, database["valid_moa_seals"].size() - 1);
    rng.seed(std::random_device()());
    std::string seal = database["valid_moa_seals"][randi(rng)].as<std::string>();
    // Issuing city
    std::string issuing_city;
    if (entrant->country == "arstotzka") {
      randi = RandomInt(0, database["issuing_city"][entrant->country].size() - 1);
      rng.seed(std::random_device()());
      issuing_city = database["issuing_city"][entrant->country][randi(rng)].as<std::string>();
    } else {
      issuing_city = entrant->city;
    }
    // Expiration date
    time_t dmin = 406944000;
    time_t dmax = 473299200;
    randi = RandomInt(dmin, dmax);
    time_t gd = randi(rng);
    struct tm* tm = localtime(&gd);
    Date expiration_date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
    // Other hair on picture
    std::string hair = entrant->pic.hair;
    std::string facial_hair = entrant->pic.facial_hair;
    std::string vision = entrant->pic.vision;
    std::string other = entrant->pic.other;
    // Change picture
    if (entrant->sex == MALE) {
      randi = RandomInt(0, database["male_hair"].size() - 1);
      rng.seed(std::random_device()());
      hair = database["male_hair"][randi(rng)].as<std::string>();
    } else {
      randi = RandomInt(0, database["female_hair"].size() - 1);
      rng.seed(std::random_device()());
      hair = database["female_hair"][randi(rng)].as<std::string>();
    }
    Picture picture(hair, facial_hair, vision, other);

    // If the entrant is illegal
    if (illegal) {
      randi = RandomInt(0, 7);
      rng.seed(std::random_device()());
      switch (randi(rng)) {
        case 0:
          // Firstname
          do {
            // Random invalid firstname
            rng.seed(std::random_device()());
            randi = RandomInt(0, 1);
            if (randi(rng) == 1) {
              // Male firstnames
              randi = RandomInt(0, database["male_firstnames"].size() - 1);
              rng.seed(std::random_device()());
              firstname = database["male_firstnames"][randi(rng)].as<std::string>();
            } else {
              // Female firstnames
              randi = RandomInt(0, database["female_firstnames"].size() - 1);
              rng.seed(std::random_device()());
              firstname = database["female_firstnames"][randi(rng)].as<std::string>();
            }
          } while(firstname == entrant->firstname);
          break;
        case 1:
          // Lastnames
          do {
            randi = RandomInt(0, database["lastnames"].size() - 1);
            rng.seed(std::random_device()());
            lastname = database["lastnames"][randi(rng)].as<std::string>();
          } while(lastname == entrant->lastname);
          break;
        case 2:
          // Sex
          if (entrant->sex == MALE) {
            sex = FEMALE;
          } else {
            sex = MALE;
          }
        case 3:
          {
            // Seal
            randi = RandomInt(0, database["forged_moa_seals"].size() - 1);
            rng.seed(std::random_device()());
            std::string seal = database["forged_moa_seals"][randi(rng)].as<std::string>();
            break;
          }
        case 4:
          {
            // Picture
            do {
              rng.seed(std::random_device()());
              randi = RandomInt(0, 1);
              if (randi(rng) == 1) {
                randi = RandomInt(0, database["male_hair"].size() - 1);
                rng.seed(std::random_device()());
                hair = database["male_hair"][randi(rng)].as<std::string>();
              } else {
                randi = RandomInt(0, database["female_hair"].size() - 1);
                rng.seed(std::random_device()());
                hair = database["female_hair"][randi(rng)].as<std::string>();
              }
            } while(hair == entrant->pic.hair);
            do {
              randi = RandomInt(0, database["facial_hair"].size() - 1);
              rng.seed(std::random_device()());
              facial_hair = database["facial_hair"][randi(rng)].as<std::string>();
            } while(facial_hair == entrant->pic.facial_hair);
            do {
              randi = RandomInt(0, database["vision"].size() - 1);
              rng.seed(std::random_device()());
              vision = database["vision"][randi(rng)].as<std::string>();
            } while(vision == entrant->pic.vision);
            do {
              randi = RandomInt(0, database["other"].size() - 1);
              rng.seed(std::random_device()());
              other = database["other"][randi(rng)].as<std::string>();
            } while(other == entrant->pic.other);
            break;
          }
        case 5:
          // Country
          do {
            randi = RandomInt(0, database["countries"].size() - 1);
            rng.seed(std::random_device()());
            country = database["countries"][randi(rng)].as<std::string>();
          } while(country == entrant->country);
          break;
        case 6:
          // Issuing city
          random_shuffle(issuing_city.begin(), issuing_city.end());
          break;
        case 7:
          {
            // Expiration date
            time_t dmin = 86400;
            time_t dmax = 604800;
            randi = RandomInt(dmin, dmax);
            time_t gd = 406944000 - randi(rng);
            struct tm* tm = localtime(&gd);
            expiration_date = Date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
            break;
          }
      }
      // Update invalid fields
      invalid_fields--;
      if (invalid_fields == 0) {
        illegal = false;
      }
    }

    picture = Picture(hair, facial_hair, vision, other);

    return (new Passport(passport_number, seal, picture, firstname, lastname,
      sex, date_of_birth, country, issuing_city, expiration_date));
}

IDCard* PEG::new_idcard(YAML::Node database, Entrant* entrant, bool illegal,
  int& invalid_fields) {
    // Random variables
    std::mt19937 rng;
    RandomInt randi;

    // Fields
    std::string firstname = entrant->firstname;
    std::string lastname = entrant->lastname;
    char sex = entrant->sex;
    Date date_of_birth = entrant->date_of_birth;
    std::string country = entrant->country;
    std::string city = entrant->city;
    float height = entrant->height;
    unsigned int weight = entrant->weight;
    // Other hair on picture
    std::string hair = entrant->pic.hair;
    std::string facial_hair = entrant->pic.facial_hair;
    std::string vision = entrant->pic.vision;
    std::string other = entrant->pic.other;
    // Change picture
    if (entrant->sex == MALE) {
      randi = RandomInt(0, database["male_hair"].size() - 1);
      rng.seed(std::random_device()());
      hair = database["male_hair"][randi(rng)].as<std::string>();
    } else {
      randi = RandomInt(0, database["female_hair"].size() - 1);
      rng.seed(std::random_device()());
      hair = database["female_hair"][randi(rng)].as<std::string>();
    }
    Picture picture(hair, facial_hair, vision, other);

    // If the entrant is illegal
    if (illegal) {
      randi = RandomInt(0, 7);
      rng.seed(std::random_device()());
      switch (randi(rng)) {
        case 0:
          // Firstname
          do {
            // Random invalid firstname
            rng.seed(std::random_device()());
            randi = RandomInt(0, 1);
            if (randi(rng) == 1) {
              // Male firstnames
              randi = RandomInt(0, database["male_firstnames"].size() - 1);
              rng.seed(std::random_device()());
              firstname = database["male_firstnames"][randi(rng)].as<std::string>();
            } else {
              // Female firstnames
              randi = RandomInt(0, database["female_firstnames"].size() - 1);
              rng.seed(std::random_device()());
              firstname = database["female_firstnames"][randi(rng)].as<std::string>();
            }
          } while(firstname == entrant->firstname);
          break;
        case 1:
          // Lastnames
          do {
            randi = RandomInt(0, database["lastnames"].size() - 1);
            rng.seed(std::random_device()());
            lastname = database["lastnames"][randi(rng)].as<std::string>();
          } while(lastname == entrant->lastname);
          break;
        case 2:
          // Sex
          if (entrant->sex == MALE) {
            sex = FEMALE;
          } else {
            sex = MALE;
          }
        case 3:
          // Country
          do {
            randi = RandomInt(0, database["countries"].size() - 1);
            rng.seed(std::random_device()());
            country = database["countries"][randi(rng)].as<std::string>();
          } while(country == entrant->country);
          break;
        case 4:
          // City
          do {
            randi = RandomInt(0, database["cities"][country].size() - 1);
            rng.seed(std::random_device()());
            city = database["cities"][country][randi(rng)].as<std::string>();
          } while(city == entrant->city);
          break;
        case 5:
          // Height
          do {
            rng.seed(std::random_device()());
            randi = RandomInt((int) (database["min_height"].as<float>() * 100),
              (int) (database["max_height"].as<float>() * 100));
            height = (randi(rng) / (float) 100);
          } while(height == entrant->height);
          break;
        case 6:
          // Weight
          do {
            rng.seed(std::random_device()());
            randi = RandomInt((int) (database["min_weight"].as<float>() * 100),
              (int) (database["max_weight"].as<float>() * 100));
            weight = randi(rng);
          } while(weight == entrant->weight);
          break;
        case 7:
          {
            // Picture
            do {
              rng.seed(std::random_device()());
              randi = RandomInt(0, 1);
              if (randi(rng) == 1) {
                randi = RandomInt(0, database["male_hair"].size() - 1);
                rng.seed(std::random_device()());
                hair = database["male_hair"][randi(rng)].as<std::string>();
              } else {
                randi = RandomInt(0, database["female_hair"].size() - 1);
                rng.seed(std::random_device()());
                hair = database["female_hair"][randi(rng)].as<std::string>();
              }
            } while(hair == entrant->pic.hair);
            do {
              randi = RandomInt(0, database["facial_hair"].size() - 1);
              rng.seed(std::random_device()());
              facial_hair = database["facial_hair"][randi(rng)].as<std::string>();
            } while(facial_hair == entrant->pic.facial_hair);
            do {
              randi = RandomInt(0, database["vision"].size() - 1);
              rng.seed(std::random_device()());
              vision = database["vision"][randi(rng)].as<std::string>();
            } while(vision == entrant->pic.vision);
            do {
              randi = RandomInt(0, database["other"].size() - 1);
              rng.seed(std::random_device()());
              other = database["other"][randi(rng)].as<std::string>();
            } while(other == entrant->pic.other);
            break;
          }
      }
      // Update invalid fields
      invalid_fields--;
      if (invalid_fields == 0) {
        illegal = false;
      }
    }

    picture = Picture(hair, facial_hair, vision, other);

    return (new IDCard(picture, firstname, lastname, sex, date_of_birth,
      country, city, height, weight));
}

AccessPermit* PEG::new_accesspermit(YAML::Node database, Entrant* entrant,
  Passport* passport, bool illegal, int& invalid_fields) {
    // Random variables
    std::mt19937 rng;
    RandomInt randi;

    // Physical appearance
    std::vector<std::string> pa;
    if (entrant->sex == MALE) {
      pa.push_back(entrant->pic.hair);
      pa.push_back(entrant->pic.facial_hair);
      pa.push_back(entrant->pic.vision);
      pa.push_back(entrant->pic.other);
    } else {
      pa.push_back(entrant->pic.hair);
      pa.push_back(entrant->pic.vision);
      pa.push_back(entrant->pic.other);
    }

    // Entrant's data
    std::string passport_number = passport->passport_number;
    std::string firstname = entrant->firstname;
    std::string lastname = entrant->lastname;
    float height = entrant->height;
    unsigned int weight = entrant->weight;
    std::string country = entrant->country;
    std::string purpose = entrant->purpose;
    // Seal
    randi = RandomInt(0, database["valid_moa_seals"].size() - 1);
    rng.seed(std::random_device()());
    std::string seal = database["valid_moa_seals"][randi(rng)].as<std::string>();
    // Expiration date
    time_t dmin = 406944000;
    time_t dmax = 473299200;
    randi = RandomInt(dmin, dmax);
    time_t gd = randi(rng);
    struct tm* tm = localtime(&gd);
    Date expiration_date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
    // Physical appearance
    randi = RandomInt(0, pa.size() - 1);
    rng.seed(std::random_device()());
    std::string physical_appearance = pa[randi(rng)];
    // Duration
    Duration duration;
    if (purpose == "Visit") {
      time_t dmin = 1209600;
      time_t dmax = 7862400;
      randi = RandomInt(dmin, dmax);
      time_t gd = randi(rng);
      struct tm* tm = localtime(&gd);
      duration = Date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
    }
    if (purpose == "Transit") {
      time_t dmin = 172800;
      time_t dmax = 1209600;
      randi = RandomInt(dmin, dmax);
      time_t gd = randi(rng);
      struct tm* tm = localtime(&gd);
      duration = Date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
    }
    if (purpose == "Work") {
      time_t dmin = 7862400;
      time_t dmax = 31622400;
      randi = RandomInt(dmin, dmax);
      time_t gd = randi(rng);
      struct tm* tm = localtime(&gd);
      duration = Date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
    }

    // If the entrant is illegal
    if (illegal) {
      randi = RandomInt(0, 9);
      rng.seed(std::random_device()());
      switch (randi(rng)) {
        case 0:
          // Passport number
          do {
            char s[10];
            static const char alphanum[] =
              "0123456789"
              "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            for (int i = 0; i < 10; ++i) {
              s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
            }
            passport_number = std::string(s);
          } while(passport_number == passport->passport_number);
          break;
        case 1:
          {
            // Seal
            randi = RandomInt(0, database["forged_moa_seals"].size() - 1);
            rng.seed(std::random_device()());
            std::string seal = database["forged_moa_seals"][randi(rng)].as<std::string>();
            break;
          }
        case 2:
          {
            // Expiration date
            time_t dmin = 86400;
            time_t dmax = 604800;
            randi = RandomInt(dmin, dmax);
            time_t gd = 406944000 - randi(rng);
            struct tm* tm = localtime(&gd);
            expiration_date = Date(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);
            break;
          }
        case 3:
          // Physical appearance
          do {
            rng.seed(std::random_device()());
            randi = RandomInt(0, 4);
            switch (randi(rng)) {
              case 0:
                randi = RandomInt(0, database["male_hair"].size() - 1);
                rng.seed(std::random_device()());
                physical_appearance = database["male_hair"][randi(rng)].as<std::string>();
                break;
              case 1:
                randi = RandomInt(0, database["female_hair"].size() - 1);
                rng.seed(std::random_device()());
                physical_appearance = database["female_hair"][randi(rng)].as<std::string>();
                break;
              case 2:
                randi = RandomInt(0, database["facial_hair"].size() - 1);
                rng.seed(std::random_device()());
                physical_appearance = database["facial_hair"][randi(rng)].as<std::string>();
                break;
              case 3:
                randi = RandomInt(0, database["vision"].size() - 1);
                rng.seed(std::random_device()());
                physical_appearance = database["vision"][randi(rng)].as<std::string>();
                break;
              case 4:
                randi = RandomInt(0, database["other"].size() - 1);
                rng.seed(std::random_device()());
                physical_appearance = database["other"][randi(rng)].as<std::string>();
                break;
            }
          } while(std::find(pa.begin(), pa.end(), physical_appearance) != pa.end());
          break;
        case 4:
          // Firstname
          do {
            // Random invalid firstname
            rng.seed(std::random_device()());
            randi = RandomInt(0, 1);
            if (randi(rng) == 1) {
              // Male firstnames
              randi = RandomInt(0, database["male_firstnames"].size() - 1);
              rng.seed(std::random_device()());
              firstname = database["male_firstnames"][randi(rng)].as<std::string>();
            } else {
              // Female firstnames
              randi = RandomInt(0, database["female_firstnames"].size() - 1);
              rng.seed(std::random_device()());
              firstname = database["female_firstnames"][randi(rng)].as<std::string>();
            }
          } while(firstname == entrant->firstname);
          break;
        case 5:
          // Lastnames
          do {
            randi = RandomInt(0, database["lastnames"].size() - 1);
            rng.seed(std::random_device()());
            lastname = database["lastnames"][randi(rng)].as<std::string>();
          } while(lastname == entrant->lastname);
          break;
        case 6:
          // Height
          do {
            rng.seed(std::random_device()());
            randi = RandomInt((int) (database["min_height"].as<float>() * 100),
              (int) (database["max_height"].as<float>() * 100));
            height = (randi(rng) / (float) 100);
          } while(height == entrant->height);
          break;
        case 7:
          // Weight
          do {
            rng.seed(std::random_device()());
            randi = RandomInt((int) (database["min_weight"].as<float>() * 100),
              (int) (database["max_weight"].as<float>() * 100));
            weight = randi(rng);
          } while(weight == entrant->weight);
          break;
        case 8:
          // Country
          do {
            randi = RandomInt(0, database["countries"].size() - 1);
            rng.seed(std::random_device()());
            country = database["countries"][randi(rng)].as<std::string>();
          } while(country == entrant->country);
          break;
        case 9:
          // Purpose
          do {
            randi = RandomInt(0, database["purpose"].size() - 1);
            rng.seed(std::random_device()());
            purpose = database["purpose"][randi(rng)].as<std::string>();
          } while(purpose == entrant->purpose);
          break;
      }
      // Update invalid fields
      invalid_fields--;
      if (invalid_fields == 0) {
        illegal = false;
      }
    }

    return (new AccessPermit(firstname, lastname, seal, country, passport_number,
      purpose, duration, height, weight, expiration_date, physical_appearance));
}

WorkPass* PEG::new_workpass(YAML::Node database, Entrant* entrant,
  AccessPermit* accesspermit, bool illegal, int& invalid_fields) {
    // Random variables
    std::mt19937 rng;
    RandomInt randi;

    // Work end date
    Duration d = accesspermit->duration;
    struct tm aux;
    aux.tm_year = d.year - 1900;
    aux.tm_mon = d.month - 1;
    aux.tm_mday = d.day;
    time_t tend = 406944000 + mktime(&aux);
    struct tm* tm = localtime(&tend);
    Date end(tm->tm_mday, tm->tm_mon, tm->tm_year + 1900);

    // Work field
    randi = RandomInt(0, database["works"].size() - 1);
    rng.seed(std::random_device()());
    std::string field = database["works"][randi(rng)].as<std::string>();

    // Seal
    randi = RandomInt(0, database["valid_mol_seals"].size() - 1);
    rng.seed(std::random_device()());
    std::string seal = database["valid_mol_seals"][randi(rng)].as<std::string>();

    // If the entrant is illegal
    if (illegal) {
      // Seal
      randi = RandomInt(0, database["forged_mol_seals"].size() - 1);
      rng.seed(std::random_device()());
      seal = database["forged_mol_seals"][randi(rng)].as<std::string>();
      // Update invalid fields
      invalid_fields--;
      if (invalid_fields == 0) {
        illegal = false;
      }
    }

    return (new WorkPass(entrant->firstname, entrant->lastname, field, seal, end));
}