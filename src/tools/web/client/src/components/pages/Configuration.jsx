/**
 * @file
 *
 * @brief this is the configuration page
 *
 * it renders the interactive tree view
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 */

import React, { Component } from 'react'

import { Card, CardHeader, CardText, CardActions } from 'material-ui/Card'
import FlatButton from 'material-ui/FlatButton'
import IconButton from 'material-ui/IconButton'
import NavigationRefresh from 'material-ui/svg-icons/navigation/refresh'
import { Link } from 'react-router-dom'

import TreeView from '../../containers/ConnectedTreeView'

// create tree structure from kdb ls result (list of available keys)
const partsTree = (acc, parts) => {
  if (parts.length <= 0) return acc

  const part = parts.shift()
  if (!acc[part]) acc[part] = {}
  partsTree(acc[part], parts)
  return acc
}

const createTree = (ls) =>
  ls.reduce((acc, item) => {
    return partsTree(acc, item.split('/'))
  }, {})

const parseDataSet = (getKey, instanceId, tree, path) => {
  return Object.keys(tree).map(key => {
    const newPath = path
      ? path + '/' + key
      : key
    const children = parseDataSet(getKey, instanceId, tree[key], newPath)
    return {
      name: key,
      path: newPath,
      children: (Array.isArray(children) && children.length > 0)
        ? () => {
          return new Promise(resolve => {
            children.map(child => getKey(instanceId, child.path))
            resolve(children)
          })
        } : false,
    }
  })
}

const parseData = (getKey, instanceId, ls, kdb) => {
  if (!Array.isArray(ls)) return
  const tree = createTree(ls)
  return parseDataSet(getKey, instanceId, tree)
}

// configuration page
export default class Configuration extends Component {
  constructor (props, ...rest) {
    super(props, ...rest)
    const { getKdb, match } = props
    const { id } = match && match.params
    getKdb(id)
    this.state = { data: this.generateData(props) || [] }
  }

  componentWillReceiveProps (nextProps) {
    this.setState({ data: this.generateData(nextProps) || [] })
  }

  updateData = (data) => {
    return this.setState({ data })
  }

  waitForData = () => {
    const { data } = this.state
    const user = Array.isArray(data) && data.find(d => d.path === 'user')
    if (!user) {
      this.timeout = setTimeout(this.waitForData, 100)
    } else {
      this.preload(data).then(this.updateData)
    }
  }

  componentDidMount () {
    this.waitForData()
  }

  generateData = ({ ls, match, getKey }) => {
    const { id } = match && match.params
    return parseData(getKey, id, ls)
  }

  refresh = () => {
    const { data } = this.state
    const { getKdb, match, sendNotification } = this.props
    const { id } = match && match.params

    sendNotification('refreshing configuration data...')
    getKdb(id)
      .then(() =>
        this.preload(data).then(this.updateData)
      )
      .then(() => sendNotification('configuration data refreshed!'))
  }

  preload = async tree => {
    if (!tree) return Promise.resolve(tree)
    return await Promise.all(tree.map(async item => {
      // do not preload system/ namespace
      if (item.name === 'system') return item

      let { children } = item

      if (!children) return item
      children = await this.preload(
        typeof children === 'function'
          ? await children()
          : children
      )

      return { ...item, children }
    }))
  }

  render () {
    const { instance, match } = this.props
    const { data } = this.state

    if (!instance) {
      const title = (
          <h1><b>404</b> instance not found</h1>
      )
      return (
          <Card>
              <CardHeader title={title} />
          </Card>
      )
    }

    const { id } = match && match.params
    const { name, host } = instance

    const title = (
        <h1>
            <b>{name}</b>{' instance'}
            <IconButton
              className="refreshIcon"
              style={{ marginLeft: 6, width: 28, height: 28, padding: 6 }}
              iconStyle={{ width: 16, height: 16 }}
              onClick={this.refresh}
              tooltip="refresh"
            >
                <NavigationRefresh />
            </IconButton>
        </h1>
    )

    return (
        <Card style={{ padding: '8px 16px' }}>
            <CardHeader title={title} subtitle={host} />
            <CardText>
                {(data && Array.isArray(data) && data.length > 0)
                  ? <TreeView
                      instanceId={id}
                      data={data}
                    />
                  : <div style={{ fontSize: '1.1em', color: 'rgba(0, 0, 0, 0.4)' }}>
                        Loading configuration data...
                    </div>
                }
            </CardText>
            <CardActions>
                <Link to="/" style={{ textDecoration: 'none' }}>
                    <FlatButton primary label="done" />
                </Link>
            </CardActions>
        </Card>
    )
  }
}
