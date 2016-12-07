import makeLog from './log'
const { info, error } = makeLog()

import { name as packageName, version as packageVersion } from '../package.json'
import getVersions from './versions'
import initApp from './app'

info(`%s v%s starting`, packageName, packageVersion)
getVersions()
  .then(versions => {
    if (!versions.elektra) {
      error(`couldn't detect elektra version`)
      error(`are you sure you have libelektra and kdb installed?`)
      process.exit(1)
    } else {
      info(`|- versions: %o`, versions)
      initApp(port => info(`\`-> running on http://localhost:${port}`))
    }
  })
  .catch(err => error(`error while starting %s: %o`, packageName, err))