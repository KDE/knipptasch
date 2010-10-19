# This generates a .ts from a .po file
macro(generate_ts outvar basename)
  set(input ${CMAKE_SOURCE_DIR}/po/${basename}.po)
  set(output ${CMAKE_BINARY_DIR}/po/${basename}.ts)
  add_custom_command(OUTPUT ${output}
          COMMAND ${QT_LCONVERT_EXECUTABLE}
          ARGS -i ${input}
               -of ts
               -o ${output}
# This is a workaround to add (duplicate) strings that lconvert missed to the .ts
          COMMAND ${QT_LUPDATE_EXECUTABLE}
          ARGS -silent
	       ${CMAKE_SOURCE_DIR}/src/
               -ts ${output}
          DEPENDS ${CMAKE_SOURCE_DIR}/po/${basename}.po)
  set(${outvar} ${output})
endmacro(generate_ts outvar basename)

# This generates a .qm from a .ts file
macro(generate_qm outvar basename)
  set(input ${CMAKE_BINARY_DIR}/po/${basename}.ts)
  set(output ${CMAKE_BINARY_DIR}/po/${basename}.qm)
  add_custom_command(OUTPUT ${output}
          COMMAND ${QT_LRELEASE_EXECUTABLE}
          ARGS -silent
               ${input}
          DEPENDS ${CMAKE_BINARY_DIR}/po/${basename}.ts)
  set(${outvar} ${output})
endmacro(generate_qm outvar basename)
