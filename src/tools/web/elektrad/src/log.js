import dude from 'debug-dude'

import { name } from '../package.json'
export const namespace = name.split('/').pop()

export default function makeLog (name) {
  const appendName = name ? (':' + name) : ''
  return dude(namespace + appendName)
}