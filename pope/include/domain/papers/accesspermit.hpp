/*
  This file is part of Pope.

  Copyright (c) 2018 by Breno Viana

  Pope is a free software; you can redistribute it and/or modify it under the
  terms of the MIT License.
*/

#ifndef _POPE_ACCESSPERMIT_HPP_
#define _POPE_ACCESSPERMIT_HPP_

#include "paper.hpp"
#include "date.hpp"

/*!
 * \brief This class represents an access permit
 */
class AccessPermit : public Paper {
  public:
    std::string moa_stamp; ///< Ministry of admission stamp
    std::string nationality; ///< Nationality
    std::string passport_number; ///< Passport number
    std::string purpose; ///< Purpose
    unsigned int duration; ///< Duration of stay in days
    float height; ///< Entrant height
    unsigned int weight; ///< Entrant weight
    Date expiration_date; ///< Expiration date
    std::string physical_appearance; ///< Physical appearance

    /*!
     * \brief Access permit empty constructor
     */
    AccessPermit() { /* empty */ }

    /*!
     * \brief Access permit constructor
     *
     * \param firstname
     *     Entrant's firstname
     * \param lastname
     *     Entrant's lastname
     * \param moa_stamp
     *     Ministry of admission stamp
     * \param nationality
     *     Nationality
     * \param passport_number
     *     Passport number
     * \param purpose
     *     Purpose
     * \param duration
     *     Duration of stay in days
     * \param height
     *     Entrant height
     * \param weight
     *     Entrant weight
     * \param expiration_date
     *     Expiration date
     * \param physical_appearance
     *     Physical appearance
     */
    AccessPermit(std::string firstname, std::string lastname, std::string moa_stamp,
      std::string nationality, std::string passport_number, std::string purpose,
      unsigned int duration, float height, unsigned int weight, Date expiration_date,
      std::string physical_appearance) : Paper(firstname, lastname) {
        this->moa_stamp = moa_stamp;
        this->nationality = nationality;
        this->passport_number = passport_number;
        this->purpose = purpose;
        this->duration = duration;
        this->height = height;
        this->weight = weight;
        this->expiration_date = expiration_date;
        this->physical_appearance = physical_appearance;
    }

  private:
    void paper() { /* empty */ }
};

#endif /* _POPE_ACCESSPERMIT_HPP_ */
