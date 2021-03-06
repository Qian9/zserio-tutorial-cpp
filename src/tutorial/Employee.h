/**
 * Automatically generated by Zserio C++ extension version 2.0.0.
 */

#ifndef TUTORIAL_EMPLOYEE_H
#define TUTORIAL_EMPLOYEE_H

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/PreWriteAction.h>
#include <zserio/OptionalHolder.h>
#include <string>
#include <vector>
#include <zserio/Arrays.h>
#include <zserio/Types.h>

#include <tutorial/Experience.h>
#include <tutorial/Role.h>

namespace tutorial
{

class Employee
{
public:
    Employee() noexcept;

    template <typename ZSERIO_T_name,
            typename ZSERIO_T_bonus,
            typename ZSERIO_T_skills>
    explicit Employee(
            uint8_t age_,
            ZSERIO_T_name&& name_,
            uint16_t salary_,
            ZSERIO_T_bonus&& bonus_,
            ::tutorial::Role role_,
            ZSERIO_T_skills&& skills_) :
            m_age_(age_),
            m_name_(::std::forward<ZSERIO_T_name>(name_)),
            m_salary_(salary_),
            m_bonus_(::std::forward<ZSERIO_T_bonus>(bonus_)),
            m_role_(role_),
            m_skills_(::std::forward<ZSERIO_T_skills>(skills_))
    {
    }

    explicit Employee(::zserio::BitStreamReader& in);

    ~Employee() = default;

    Employee(const Employee&) = default;
    Employee& operator=(const Employee&) = default;

    Employee(Employee&&) = default;
    Employee& operator=(Employee&&) = default;

    uint8_t getAge() const;
    void setAge(uint8_t age_);

    ::std::string& getName();
    const ::std::string& getName() const;
    void setName(const ::std::string& name_);
    void setName(::std::string&& name_);

    uint16_t getSalary() const;
    void setSalary(uint16_t salary_);

    uint16_t getBonus() const;
    void setBonus(uint16_t bonus_);
    void resetBonus();
    bool hasBonus() const;

    ::tutorial::Role getRole() const;
    void setRole(::tutorial::Role role_);

    ::std::vector<::tutorial::Experience>& getSkills();
    const ::std::vector<::tutorial::Experience>& getSkills() const;
    void setSkills(const ::std::vector<::tutorial::Experience>& skills_);
    void setSkills(::std::vector<::tutorial::Experience>&& skills_);
    void resetSkills();
    bool hasSkills() const;

    size_t bitSizeOf(size_t bitPosition = 0) const;
    size_t initializeOffsets(size_t bitPosition);

    bool operator==(const Employee& other) const;
    int hashCode() const;

    void read(::zserio::BitStreamReader& in);
    void write(::zserio::BitStreamWriter& out,
            ::zserio::PreWriteAction preWriteAction = ::zserio::ALL_PRE_WRITE_ACTIONS);

private:
    class ElementFactory_skills;

    uint8_t readAge(::zserio::BitStreamReader& in);
    ::std::string readName(::zserio::BitStreamReader& in);
    uint16_t readSalary(::zserio::BitStreamReader& in);
    ::zserio::OptionalHolder<uint16_t> readBonus(::zserio::BitStreamReader& in);
    ::tutorial::Role readRole(::zserio::BitStreamReader& in);
    ::zserio::OptionalHolder<::std::vector<::tutorial::Experience>> readSkills(::zserio::BitStreamReader& in);

    uint8_t m_age_;
    ::std::string m_name_;
    uint16_t m_salary_;
    ::zserio::OptionalHolder<uint16_t> m_bonus_;
    ::tutorial::Role m_role_;
    ::zserio::OptionalHolder<::std::vector<::tutorial::Experience>> m_skills_;
};

} // namespace tutorial

#endif // TUTORIAL_EMPLOYEE_H
