## Example: Skipable Dictionary

The following example generates a dictionary which can be partially parsed and entries can be skipped if not known to the application:

#### Steps

- Generate zserio packages without any imports
- Generate with same top level namespace package
  `java -jar zserio.jar -setTopLevelPackage skipdict [...]`
- Generate code with CMake option: `cmake .. -DZSERIO_TUTORIAL_SKIPDICT=ON`




#### Tasks for the application

- Mapping between type code field and enum values
