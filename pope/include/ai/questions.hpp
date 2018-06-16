/*
  This file is part of Pope.

  Copyright (c) 2018 by Breno Viana

  Pope is a free software; you can redistribute it and/or modify it under the
  terms of the MIT License.
*/

#ifndef _POPE_ACTIONS_HPP_
#define _POPE_ACTIONS_HPP_

#include "domain/entrant.hpp"
#include "domain/papers/paper.hpp"
#include "services/readers/database_reader.hpp"

enum Questions {
  HAS_PASSPORT,
  CHECK_PICTURE_PASSPORT,
  CHECK_MOASTAMP_PASSPORT,
  CHECK_SEX_PASSPORT,
  CHECK_ISSUINGCITY_PASSPORT,
  CHECK_COUNTRY_PASSPORT,
  CHECK_EXPIRATIONDATE_PASSPORT,
  IS_ARSTOTZKAN,
  HAS_IDCARD,
  CHECK_PICTURE_IDCARD,
  CHECK_NAME_IDCARD_PASSPORT,
  CHECK_SEX_IDCARD,
  CHECK_DATEOFBIRTH_IDCARD,
  CHECK_CITY_IDCARD,
  CHECK_COUNTRY_IDCARD,
  CHECK_HEIGHT_IDCARD,
  CHECK_WEIGHT_IDCARD,
  HAS_ACCESSPERMIT,
  CHECK_NAME_ACCESSPERMIT_PASSPORT,
  CHECK_MOASTAMP_ACCESSPERMIT,
  CHECK_NATIONALITY_ACCESSPERMIT,
  CHECK_PASSPORTNUMBER_ACCESSPERMIT,
  CHECK_PURPOSE_ACCESSPERMIT,
  CHECK_HEIGHT_ACCESSPERMIT,
  CHECK_WEIGHT_ACCESSPERMIT,
  CHECK_EXPIRATIONDATE_ACCESSPERMIT,
  CHECK_PHYSICALAPPEARANCE_ACCESSPERMIT,
  HAS_WORKPASS,
  CHECK_NAME_WORKPASS_PASSPORT,
  CHECK_MOLSTAMP_WORKPASS,
  CHECK_FIELD_WORKPASS,
  CHECK_WORKENDDATE_WORKPASS
};

class Question {
  public:
    static bool ask(unsigned int ac, Entrant entrant, time_t current_day);

  private:
    static bool has_passport(YAML::Node database, Entrant entrant);
    static bool check_picture_passport(YAML::Node database, Entrant entrant);
    static bool check_moastamp_passport(YAML::Node database, Entrant entrant);
    static bool check_sex_passport(YAML::Node database, Entrant entrant);
    static bool check_issuingcity_passport(YAML::Node database, Entrant entrant);
    static bool check_country_passport(YAML::Node database, Entrant entrant);
    static bool check_expirationdate_passport(YAML::Node database, Entrant entrant, time_t current_day);
    static bool is_arstotzkan(YAML::Node database, Entrant entrant);
    static bool has_idcard(YAML::Node database, Entrant entrant);
    static bool check_picture_idcard(YAML::Node database, Entrant entrant);
    static bool check_name_idcard_passport(YAML::Node database, Entrant entrant);
    static bool check_sex_idcard(YAML::Node database, Entrant entrant);
    static bool check_dateofbirth_idcard(YAML::Node database, Entrant entrant);
    static bool check_city_idcard(YAML::Node database, Entrant entrant);
    static bool check_country_idcard(YAML::Node database, Entrant entrant);
    static bool check_height_idcard(YAML::Node database, Entrant entrant);
    static bool check_weight_idcard(YAML::Node database, Entrant entrant);
    static bool has_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_name_accesspermit_passport(YAML::Node database, Entrant entrant);
    static bool check_moastamp_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_nationality_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_passportnumber_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_purpose_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_height_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_weight_accesspermit(YAML::Node database, Entrant entrant);
    static bool check_expirationdate_accesspermit(YAML::Node database, Entrant entrant, time_t current_day);
    static bool check_physicalappearance_accesspermit(YAML::Node database, Entrant entrant);
    static bool has_workpass(YAML::Node database, Entrant entrant);
    static bool check_name_workpass_passport(YAML::Node database, Entrant entrant);
    static bool check_molstamp_workpass(YAML::Node database, Entrant entrant);
    static bool check_field_workpass(YAML::Node database, Entrant entrant);
    static bool check_workenddate_workpass(YAML::Node database, Entrant entrant, time_t current_day);
};

#endif /* _POPE_ACTIONS_HPP_ */
